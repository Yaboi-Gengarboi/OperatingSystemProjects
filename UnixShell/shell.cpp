// shell.cpp
// Made by Justyn Dunford and Dawson Hampton
// Started March 21st, 2022
// Last modified on March 31st, 2022
// Note: Requires C++17 or later

//#if defined(__unix__)

#include <filesystem>
using std::filesystem::current_path;

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <sstream>
using std::istringstream;

#include <string>
using std::size_t;
using std::string;
using std::getline;

#include <vector>
using std::vector;

// Prints every std::string contained in the given std::vector.
// Mostly for debug purposes.
void print(const vector<string>& vec)
{
	if (vec.empty())
	{
		cout << "{ }" << endl;
		return;
	}

	cout << "{ ";

	for (size_t i = 0; i < vec.size() - 1; ++i)
		cout << vec[i] << ", ";

	cout << vec.back() << " }" << endl;
}

// Fills up the given std::vector<string> with
// tokens contained within the given std::string
// seperated by ' ' characters.
// The given std::vector is emptied before it is refilled.
// Returns the amount of tokens put into the std::vector.
size_t get_tokens(vector<string>& tokens, string& line)
{
	string token;
	istringstream stream(line);

	tokens.clear();

	while (stream >> token)
		tokens.push_back(token);

	return tokens.size();
}

// Prints the current directory followed by "> ".
void print_directory()
{
	cout << current_path() << "> ";
}

// Main loop for the shell.
// Currently only loops once.
void shell_loop()
{
	string line(100, ' ');
	vector<string> tokens;
	bool stop = false;
	size_t begin = 0;
	size_t pos = 0;

	while (!stop)
	{
		print_directory();

		getline(cin, line);

		if (line.size() > 100)
		{
			cout << "ERROR: Cannot parse line greater than 100 characters." << endl;
			line.resize(100);
		}

		get_tokens(tokens, line);
		print(tokens);
		stop = true;
	}
}

int main(int argc, char** argv)
{
	shell_loop();
	return 0;
}

//#endif // #if defined(__unix__)