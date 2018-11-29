#include <iostream>
#include <CL/cl.h>


int main(void)
{
	cl_int err;
	cl_uint numPlatforms;

	err = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (CL_SUCCESS == err)
		std::cout << "\nDetected OpenCL platforms: " << numPlatforms << std::endl;
	else
		std::cout << "\nError calling clGetPlatformIDs. Error code: " << err << std::endl;

	return 0;
}