
#include "prec.h"

const char unknow_msg[] = "Unknown command: ";
class continue_signal {};

typedef Matrix<Frac<unsigned int>> matrix_t;

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

const char * help_msg[] = {
	"Matrix calculator by decdl",
	"Commands:",
	"	\e[1mhelp\e[0m:	show this message",
	"	\e[1mexit\e[0m:	quit the program",
	"	\e[1mref\e[0m:	reduce matrix to reduced echelon form",
	"	\e[1mdet\e[0m:	calculate determinant",
	"	\e[1mmul\e[0m:	matrix multiplication"
};

const struct
{
	std::string name;
	void (*function)();
} commands[] = {
	
	{"help", []()
		{
			for (const char *msg : help_msg)
				std::cout << msg << std::endl;
			throw continue_signal();
		}
	},

	{"ref", []()
		{
			matrix_t A;
			std::cin >> A;
			std::cout << A.ref() << std::endl;
			throw continue_signal();
		}
	},

	{"det", []()
		{
			matrix_t A;
			std::cin >> A;
			std::cout << A.det() << std::endl;
			throw continue_signal();
		}
	},

	{"mul", []()
		{
			matrix_t A, B;
			std::cin >> A >> B;
			std::cout << A*B << std::endl;
			throw continue_signal();
		}
	}
};

int main()
{
	std::cin.exceptions(std::ios::failbit);
	std::string cmd;
	while (true)
	{
		try
		{
			if (prompt(cmd))
				return 0;
			for (const decltype(commands[0]) &command : commands)
				if (cmd == command.name)
					command.function();
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
