
#include "precompile.h"

int main()
{
	matrix<frac<uint64_t>> A;
	std::wcin >> A;
	std::wcout << A.ref() << std::endl;
	std::wcout << A.det() << std::endl;
	return 0;
}
