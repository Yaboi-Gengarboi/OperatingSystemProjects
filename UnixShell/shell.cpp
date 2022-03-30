// shell.cpp
// Made by Justyn Dunford and Dawson Hampton
// Started March 21st, 2022
// Last modified on March 30th, 2022
// Note: Requires C++17 or later

//#if defined(__unix__)

#include <filesystem>
using std::filesystem::current_path;

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;
using std::getline;

#include <vector>
using std::vector;

// Prints the current directory followed by "> ".
void print_directory()
{
	cout << current_path() << "> ";
}

// Main loop for the shell.
// Currently only loops once.
void shell_loop()
{
	string line;
	vector<string> args;
	bool stop = false;

	while (!stop)
	{
		print_directory();
		getline(cin, line);
		cout << line << endl;
		stop = true;
	}
}

int main(int argc, char** argv)
{
	shell_loop();
	return 0;
}

//#endif // #if defined(__unix__)