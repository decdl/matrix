
#include "prec.h"

int main()
{
	try
	{
		Matrix<Frac<uint64_t>> A;
		std::cin >> A;
		std::cout << A.ref() << std::endl;
		std::cout << A.det() << std::endl;
	}
	catch (const std::exception &error)
	{
		std::cerr << "\e[31mException: " << error.what() << "\e[0m" << std::endl;
	}
	return 0;
}
