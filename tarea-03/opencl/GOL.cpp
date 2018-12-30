#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <CL/cl.h>
#include <time.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>

#define SRAND_VALUE 1985
#define LOCAL_SIZE 32
#define GOL_IF 0	// Ejecutar el juego preguntando con IF (0:Falso 1:Verdadero)
#define IMPRIMIR 0  // Imprimir o no las matrices de entrada y de salida
#define T_LIMIT 1	// Tiempo límite de cálculo

void imprimir(int *matriz, int n, int m);

int main(int argc, char *argv[]) {

	// Carga NxM desde un archivo
	std::ifstream infile;
	infile.open("NxM.txt");
	int x;
	int N = 0;
	int M = 0;
	int jfile = 0;
	while (infile >> x) {
		if (jfile == 0) { N = x; }
		else { M = x; }
		jfile = 1;
	}
	infile.close();
	printf("Cargando matriz %dx%d\n", N, M);

	int i, j;
	int *h_grid;
	cl_mem d_grid;
	cl_mem d_newGrid;
	cl_mem d_tmpGrid;

	signed t0, t1;				// Variables para medir tiempo
	double time = 0;			// Variables para medir tiempo
	double Noperaciones = 0;	// Variable para medir cantidad de operaciones ejecutadas
	int dimFilas = N;			// Dimensiones del juego de la vida (Filas), sin contar las filas fantasmas
	int dimColumnas = M;		// Dimensiones del juego de la vida (Columnas), sin contar las columnas fantasmas

	// Tamaño, en bytes, de cada vector
	size_t bytes = sizeof(int) * (dimFilas + 2) * (dimColumnas + 2);

	// Allocate host Grid used for initial setup and read back from device
	h_grid = (int *)malloc(bytes);

	cl_platform_id cpPlatform;        // OpenCL platform
	cl_device_id device_id;           // device ID
	cl_context context;               // context
	cl_command_queue queue;           // command queue
	cl_program program;               // program
	cl_kernel k_gol, k_ghostRows, k_ghostCols, k_gol_if; // Kernels

	// Assign initial population randomly
	srand(SRAND_VALUE);
	for (i = 1; i <= dimFilas; i++) {
		for (j = 1; j <= dimColumnas; j++) {
			h_grid[i * (dimColumnas + 2) + j] = rand() % 2;
		}
	}

	cl_int err;

	// Bind to platform
	err = clGetPlatformIDs(1, &cpPlatform, NULL);
	if (err != CL_SUCCESS) {
		printf("Error: Failed to find a platform\n");
		return EXIT_FAILURE;
	}

	// Get ID for the device
	err = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
	if (err != CL_SUCCESS) {
		printf("Error: Failed to create a device group\n");
		return EXIT_FAILURE;
	}

	// Create a context
	context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
	if (!context) {
		printf("Error: Failed to create a compute context\n");
		return EXIT_FAILURE;
	}

	// Create a command queue
	queue = clCreateCommandQueue(context, device_id, 0, &err);
	if (!queue) {
		printf("Error: Failed to create a command commands\n");
		return EXIT_FAILURE;
	}

	// Create the compute program from the kernel source file
	char *fileName = "GOL-kernels.cl";
	FILE *fh = fopen(fileName, "r");
	if (!fh) {
		printf("Error: Failed to open file\n");
		return 0;
	}
	struct stat statbuf;
	stat(fileName, &statbuf);
	char *kernelSource = (char *)malloc(statbuf.st_size + 1);
	fread(kernelSource, statbuf.st_size, 1, fh);
	kernelSource[statbuf.st_size] = '\0';
	program = clCreateProgramWithSource(context, 1, (const char **)&kernelSource, NULL, &err);
	if (!program) {
		printf("Error: Failed to create compute program\n");
		return EXIT_FAILURE;
	}

	// Build the program executable
	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (err != CL_SUCCESS) {
		printf("Error: Failed to build program executable %d\n", err);

		// Determine the size of the log
		size_t log_size;
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

		// Allocate memory for the log
		char *log = (char *)malloc(log_size);

		// Get the log
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

		// Print the log
		printf("%s\n", log);

		return EXIT_FAILURE;
	}

	// Create the GOL kernel in the program we wish to run
	k_gol = clCreateKernel(program, "GOL", &err);
	if (!k_gol || err != CL_SUCCESS) {
		printf("Error: Failed to create GOL kernel \n");
		return EXIT_FAILURE;
	}

	// Create the ghostRows kernel in the program we wish to run
	k_ghostRows = clCreateKernel(program, "ghostRows", &err);
	if (!k_ghostRows || err != CL_SUCCESS) {
		printf("Error: Failed to create ghostRows kernel\n");
		return EXIT_FAILURE;
	}

	// Create the ghostCols kernel in the program we wish to run
	k_ghostCols = clCreateKernel(program, "ghostCols", &err);
	if (!k_ghostCols || err != CL_SUCCESS) {
		printf("Error: Failed to create ghostCols kernel\n");
		return EXIT_FAILURE;
	}

	// Create the GOL_IF kernel in the program we wish to run
	k_gol_if = clCreateKernel(program, "GOL_IF", &err);
	if (!k_gol || err != CL_SUCCESS) {
		printf("Error: Failed to create GOL kernel \n");
		return EXIT_FAILURE;
	}

	// Create the input and output arrays in device memory for our calculation
	d_grid = clCreateBuffer(context, CL_MEM_READ_WRITE, bytes, NULL, NULL);
	d_newGrid = clCreateBuffer(context, CL_MEM_READ_WRITE, bytes, NULL, NULL);
	if (!d_grid || !d_newGrid) {
		printf("Error: Failed to allocate device memory\n");
		return EXIT_FAILURE;
	}

	// Write our data set into the input array in device memory
	err = clEnqueueWriteBuffer(queue, d_grid, CL_TRUE, 0,
		bytes, h_grid, 0, NULL, NULL);
	if (err != CL_SUCCESS) {
		printf("Error: Failed to write to source array\n");
		return EXIT_FAILURE;
	}

	// Set the arguments to GOL kernel
	err = clSetKernelArg(k_gol, 0, sizeof(int), &dimFilas);
	err |= clSetKernelArg(k_gol, 1, sizeof(cl_mem), &d_grid);
	err |= clSetKernelArg(k_gol, 2, sizeof(cl_mem), &d_newGrid);
	err |= clSetKernelArg(k_gol, 3, sizeof(int), &dimColumnas);
	if (err != CL_SUCCESS) {
		printf("Error: Failed to set kernel arguments\n");
		return EXIT_FAILURE;
	}

	// Set the arguments to ghostRows kernel
	err = clSetKernelArg(k_ghostRows, 0, sizeof(int), &dimFilas);
	err |= clSetKernelArg(k_ghostRows, 1, sizeof(cl_mem), &d_grid);
	err |= clSetKernelArg(k_ghostRows, 2, sizeof(int), &dimColumnas);
	if (err != CL_SUCCESS) {
		printf("Error: Failed to set kernel arguments\n");
		return EXIT_FAILURE;
	}

	// Set the arguments to ghostCols kernel
	err = clSetKernelArg(k_ghostCols, 0, sizeof(int), &dimFilas);
	err |= clSetKernelArg(k_ghostCols, 1, sizeof(cl_mem), &d_grid);
	err |= clSetKernelArg(k_ghostCols, 2, sizeof(int), &dimColumnas);
	if (err != CL_SUCCESS) {
		printf("Error: Failed to set kernel arguments\n");
		return EXIT_FAILURE;
	}

	// Set the arguments to GOL_IF  kernel
	err = clSetKernelArg(k_gol_if, 0, sizeof(int), &dimFilas);
	err |= clSetKernelArg(k_gol_if, 1, sizeof(cl_mem), &d_grid);
	err |= clSetKernelArg(k_gol_if, 2, sizeof(cl_mem), &d_newGrid);
	err |= clSetKernelArg(k_gol_if, 3, sizeof(int), &dimColumnas);
	if (err != CL_SUCCESS) {
		printf("Error: Failed to set kernel arguments\n");
		return EXIT_FAILURE;
	}

	// Set kernel local and global sizes
	size_t cpyRowsGlobalSize, cpyColsGlobalSize, cpyLocalSize;
	cpyLocalSize = LOCAL_SIZE;

	// Number of total work items - localSize must be devisor
	cpyRowsGlobalSize = (size_t)ceil(dimFilas / (float)cpyLocalSize) * cpyLocalSize;
	cpyColsGlobalSize = (size_t)ceil((dimColumnas + 2) / (int)cpyLocalSize) * cpyLocalSize;

	size_t GolLocalSize[2] = { LOCAL_SIZE, LOCAL_SIZE };
	size_t linGlobal = (size_t)ceil(dimFilas / (float)LOCAL_SIZE) * LOCAL_SIZE;
	size_t GolGlobalSize[2] = { linGlobal, linGlobal };

	// Imprimimos de ser el caso
	if (IMPRIMIR) { imprimir(h_grid, N, M); }

	// Main game loop
	t0 = clock();
	int iter = 0;
	while (time < T_LIMIT) {
		err = clEnqueueNDRangeKernel(queue, k_ghostRows, 1, NULL, &cpyRowsGlobalSize, &cpyLocalSize,
			0, NULL, NULL);
		err |= clEnqueueNDRangeKernel(queue, k_ghostCols, 1, NULL, &cpyColsGlobalSize, &cpyLocalSize,
			0, NULL, NULL);
		if (!GOL_IF) {
			err |= clEnqueueNDRangeKernel(queue, k_gol_if, 2, NULL, GolGlobalSize, GolLocalSize, 0, NULL, NULL);
		}
		else {
			err |= clEnqueueNDRangeKernel(queue, k_gol, 2, NULL, GolGlobalSize, GolLocalSize, 0, NULL, NULL);
		}
		if (iter % 2 == 1) {
			err |= clSetKernelArg(k_ghostRows, 1, sizeof(cl_mem), &d_grid);
			err |= clSetKernelArg(k_ghostCols, 1, sizeof(cl_mem), &d_grid);
			if (!GOL_IF) {
				err |= clSetKernelArg(k_gol, 1, sizeof(cl_mem), &d_grid);
				err |= clSetKernelArg(k_gol, 2, sizeof(cl_mem), &d_newGrid);
			}
			else {
				err |= clSetKernelArg(k_gol_if, 1, sizeof(cl_mem), &d_grid);
				err |= clSetKernelArg(k_gol_if, 2, sizeof(cl_mem), &d_newGrid);
			}
		}
		else {
			err |= clSetKernelArg(k_ghostRows, 1, sizeof(cl_mem), &d_newGrid);
			err |= clSetKernelArg(k_ghostCols, 1, sizeof(cl_mem), &d_newGrid);
			if (!GOL_IF) {
				err |= clSetKernelArg(k_gol, 1, sizeof(cl_mem), &d_newGrid);
				err |= clSetKernelArg(k_gol, 2, sizeof(cl_mem), &d_grid);
			}
			else {
				err |= clSetKernelArg(k_gol_if, 1, sizeof(cl_mem), &d_newGrid);
				err |= clSetKernelArg(k_gol_if, 2, sizeof(cl_mem), &d_grid);
			}
		}

		Noperaciones += N * M;

		t1 = clock();
		time = ((((double)t1) - t0) / CLOCKS_PER_SEC);

	}// End main game loop

	/*    Error pero lo ejecuta de todas formas.
	if (err != CL_SUCCESS) {
	   printf("Error: Failed to launch kernels%d\n",err);
	   return EXIT_FAILURE;
	}
	*/
	// Wait for the command queue to get serviced before reading back results
	clFinish(queue);

	// Read the results from the device
	err = clEnqueueReadBuffer(queue, d_grid, CL_TRUE, 0,
		bytes, h_grid, 0, NULL, NULL);
	if (err != CL_SUCCESS) {
		printf("Error: Failed to read output array\n");
		return EXIT_FAILURE;
	}

	// Imprimimos de ser el caso
	if (IMPRIMIR) {
		printf("\n");
		imprimir(h_grid, N, M);
	}

	// Imprimimos datos pedidos
	printf("Tiempo total: %f\n", time);
	printf("Numero de operaciones efectuadas: %.0f\n", Noperaciones);

	// Release memory
	free(h_grid);

	return 0;
}

void imprimir(int *matriz, int n, int m) {
	for (int i = 1; i < n - 1; i++) {
		for (int j = 1; j < m - 1; j++) {
			printf("%d ", matriz[i * m + j]);
		}
		printf("\n");
	}
}