
#include "precompile.h"

int main()
{
	try
	{
		matrix<frac<uint64_t>> A;
		std::wcin >> A;
		std::wcout << A.ref() << std::endl;
		std::wcout << A.det() << std::endl;
	}
	catch (const wchar_t *error)
	{
		std::wcerr << error << std::endl;
	}
	return 0;
}
