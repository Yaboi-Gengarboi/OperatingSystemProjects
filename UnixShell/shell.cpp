// shell.cpp
// Made by Justyn Dunford and Dawson Hampton
// Started March 21st, 2022
// Last modified on April 4st, 2022
// Note: Requires C++17 or later

// Important: Windows will throw a hissy-fit 
// if you don't use its fancy fopen_s without this.
#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif // #ifdef _WIN32

#if defined(__unix__)

#include <unistd.h>

#endif // #if defined(__unix__)

#include <cstdio>
using std::FILE;
using std::fclose;
using std::fopen;
using std::remove;

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
using std::stoi;
using std::stod;
using std::to_string;

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

// 
bool does_file_exist(const string& path)
{
	// Check if file already exists.
	// fopen will return a valid FILE* if a file is successfully opened.
	FILE* file = fopen(path.c_str(), "r");

	if (file)
	{
		fclose(file);
		return true;
	}

	return false;
}

// 
bool create_file(const string& path)
{
	if (does_file_exist(path))
	{
		cout << "ERROR: Cannot create file: " << path << ": File already exists." << endl;
		return false;
	}

	// File does not exist, so create it.
	FILE* file = fopen(path.c_str(), "w");
	if (file)
		fclose(file);
	else
	{
		cout << "ERROR: Cannot create file: " << path << endl;
		return false;
	}

	return true;
}

// 
bool remove_file(const string& path)
{
	if (does_file_exist(path))
	{
		// remove will return a non-zero value on error.
		if (remove(path.c_str()))
		{
			cout << "ERROR: Could not remove file: " << path << endl;
			return false;
		}
	}
	else
	{
		cout << "ERROR: COuld not remove file: " << path << ": File does not exist." << endl;
		return false;
	}

	return true;
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
	cout << current_path();

	#if defined(__unix__)
		cout << "$ ";
	#endif // #if defined(__unix__)

	#ifdef _WIN32
		cout << "> ";
	#endif // #ifdef _WIN32
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

		for (size_t i = 0; i < get_tokens(tokens, line); ++i)
		{
			if (tokens[i] == "help" || tokens[i] == "commands")
			{
				cout << "Available commands: " << endl;
				cout << "createfile _filepath_: Creates a file in the current directory." << endl;
				cout << "echo _text_: Prints the given text." << endl;
				cout << "exit: Exits the terminal." << endl;
				cout << "print _text_: Prints the given text." << endl;
				cout << "removefile _filepath_: Removes the given file." << endl;
				cout << "rm _filepath_: Removes the given file." << endl;
				cout << "stop: Exits the terminal." << endl;
				cout << "touch _filepath_: Creates a file in the current directory." << endl;
				cout << "unlink _filepath_: Removes the given file." << endl;
			}
			else if (tokens[i] == "touch" || tokens[i] == "createfile")
			{
				if (i == tokens.size() - 1)
					cout << "ERROR: No file name given." << endl;
				else
					create_file(tokens[i + 1]);
			}
			else if (tokens[i] == "rm" || tokens[i] == "unlink" || tokens[i] == "removefile")
			{
				if (i == tokens.size() - 1)
					cout << "ERROR: No file name given." << endl;
				else
					remove_file(tokens[i + 1]);
			}
			else if (tokens[i] == "echo" || tokens[i] == "print")
			{
				if (i == tokens.size() - 1)
					cout << "ERROR: No text given." << endl;
				else
					cout << tokens[i + 1] << endl;
			}
			else if (tokens[i] == "exit" || tokens[i] == "stop")
				exit(0);
		}
	}
}

int main(int argc, char** argv)
{

	shell_loop();
	return 0;
}