
#include "precompile.h"

int main()
{
	matrix<frac<unsigned int>> A;
	std::wcin >> A;
	A.reduce_to_ref();
	std::wcout << A << std::endl;
	return 0;
}
