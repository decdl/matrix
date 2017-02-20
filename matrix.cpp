
#include "prec.h"

const char unknow_msg[] = "Unknown command: ";
const char * help_msg[] = {
	"Matrix calculator by decdl",
	"Commands:",
	"	\e[1mhelp\e[0m:	show this message",
	"	\e[1mexit\e[0m:	quit the program",
	"	\e[1mref\e[0m:	reduce matrix to reduced echelon form",
	"	\e[1mdet\e[0m:	calculate determinant"
};

class continue_signal {};

[[noreturn]] inline void bad_input() {throw std::ios::failure("invalid input");}

inline bool prompt(std::string &cmd)
{
	std::cout << "matrix> ";
	std::getline(std::cin, cmd);
	if (!std::cin)
		bad_input();
	if (cmd == "exit")
		return true;
	return false;
}

inline void unknown(const std::string &cmd)
{
	std::cout << "\e[31m" << unknow_msg << "\e[1m" << cmd << "\e[0m" << std::endl;
	throw continue_signal();
}

inline void help()
{
	for (const char *msg : help_msg)
		std::cout << msg << std::endl;
	throw continue_signal();
}

inline void ref()
{
	Matrix<Frac<unsigned int>> A;
	std::cin >> A;
	if (!std::cin)
		bad_input();
	std::cout << A.ref() << std::endl;
	throw continue_signal();
}

inline void det()
{
	Matrix<Frac<unsigned int>> A;
	std::cin >> A;
	if (!std::cin)
		bad_input();
	std::cout << A.det() << std::endl;
	throw continue_signal();
}

int main()
{
	std::string cmd;
	while (true)
	{
		try
		{
			if (prompt(cmd))
				return 0;
			if (cmd == "help")
				help();
			if (cmd == "ref")
				ref();
			if (cmd == "det")
				det();
			unknown(cmd);
		}
		catch (const std::invalid_argument &e)
		{
			std::cerr << "\e[31mInvalid argument: " << e.what() << "\e[0m" << std::endl;
		}
		catch (const std::ios::failure &e)
		{
			if (std::cin.eof())
			{
				std::cout << std::endl;
				return 0;
			}
			std::cerr << "\e[31mI/O Failure: " << e.what() << "\e[0m" << std::endl;
			std::cin.clear();
		}
		catch (continue_signal) {}
	}
}
