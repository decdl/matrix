
#include "precompile.h"

typedef Frac<unsigned int> data_t;

int main()
{
	std::vector<vector<data_t>> matrix;

	// input
	while (!std::wcin.eof())
	{
		// prepare line processing
		std::wstring str;
		std::getline(std::wcin, str);
		if (str.size() == 0)
			break;
		std::wistringstream iss(str);

		// create a new row
		matrix.emplace_back();
		while (!iss.eof())
		{
			data_t entry;
			iss >> entry;
			matrix.back().emplace_back(std::move(entry));
		}
	}

	size_t num_row = matrix.size(),
		   num_col = matrix.front().size();

	// transform matrix into REF
	for (size_t i = 0; i < num_row; i++)
	{
		if (i >= num_col)
			break;
		if (-1e-7 < matrix[i][i] && matrix[i][i] < 1e-7)
			continue;
		matrix[i] /= matrix[i][i];
		for (size_t j = 0; j < num_row; j++)
			if (i != j)
				matrix[j] = matrix[j] - matrix[j][i] * matrix[i];
	}

	// output
	for (const vector<data_t> &row : matrix)
	{
		bool first_entry = true;
		for (const data_t &entry : row)
		{
			if (first_entry)
				first_entry = false;
			else
				std::wcout << L'\t';
			std::wcout << entry;
		}
		std::wcout << std::endl;
	}

	return 0;
}
