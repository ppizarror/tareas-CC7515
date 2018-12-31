__kernel void ghostRows(const int dimFilas, __global int *grid, const int dimColumnas) {
	// Queremos id en [1,dim]
	int id = get_global_id(0) + 1;

	if (id <= dimColumnas)
	{
		// Copiamos la primera fila real a la ultima fila
		grid[(dimColumnas + 2)*(dimFilas + 1) + id] = grid[(dimColumnas + 2) + id];
		// Copiamos la ultima fila real a la primera fila
		grid[id] = grid[(dimColumnas + 2)*dimFilas + id];
	}
}

__kernel void ghostCols(const int dimFilas, __global int *grid, const int dimColumnas) {
	// Queremos id en [0,dim+1]
	int id = get_global_id(0);

	if (id <= dimFilas + 1)
	{
		// Copia la primera columna real a la ultima
		grid[id*(dimColumnas + 2) + dimFilas + 1] = grid[id*(dimColumnas + 2) + 1];
		// Copia la ultima columna real a la primera
		grid[id*(dimColumnas + 2)] = grid[id*(dimColumnas + 2) + dimFilas];
	}
}

__kernel void GOL(const int dimFilas, __global int *grid, __global int *newGrid, const int dimColumnas) {
	int ix = get_global_id(0) + 1;
	int iy = get_global_id(1) + 1;
	int id = iy * (dimColumnas + 2) + ix;

	int numNeighbors;
	if (iy <= dimFilas && ix <= dimColumnas) {
		// Obtenemos la cantidad de vecinos vivos
		numNeighbors = grid[id + (dimColumnas + 2)] + grid[id - (dimColumnas + 2)] // upper lower
			+ grid[id + 1] + grid[id - 1] // right left
			+ grid[id + (dimColumnas + 3)] + grid[id - (dimColumnas + 3)] // diagonals
			+ grid[id - (dimColumnas + 1)] + grid[id + (dimColumnas + 1)];

		int cell = grid[id];
		// Ponemos las reglas del juego
		if (cell == 1 && (numNeighbors == 2 || numNeighbors == 3)) {
			newGrid[id] = 1;
		}
		else if (cell == 0 && numNeighbors == 3) {
			newGrid[id] = 1;
		}
		else {
			newGrid[id] = cell;
		}
	}
}

__kernel void GOL_IF(const int dimFilas, __global int *grid, __global int *newGrid, const int dimColumnas) {
	int ix = get_global_id(0) + 1;
	int iy = get_global_id(1) + 1;
	int id = iy * (dimColumnas + 2) + ix;

	int numNeighbors = 0;

	if (iy <= dimFilas && ix <= dimColumnas) {

		// Obtenemos la cantidad de vecinos vivos
		if (grid[id + (dimColumnas + 2)]) { numNeighbors++; }
		if (grid[id - (dimColumnas + 2)]) { numNeighbors++; }
		if (grid[id + 1]) { numNeighbors++; }
		if (grid[id - 1]) { numNeighbors++; }
		if (grid[id + (dimColumnas + 3)]) { numNeighbors++; }
		if (grid[id - (dimColumnas + 3)]) { numNeighbors++; }
		if (grid[id - (dimColumnas + 1)]) { numNeighbors++; }
		if (grid[id + (dimColumnas + 1)]) { numNeighbors++; }

		int cell = grid[id];
		// Ponemos las reglas del juego
		if (cell == 1 && (numNeighbors == 2 || numNeighbors == 3)) {
			newGrid[id] = 1;
		}
		else if (cell == 0 && numNeighbors == 3) {
			newGrid[id] = 1;
		}
		else {
			newGrid[id] = cell;
		}
	}
}