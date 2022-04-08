// shell.cpp
// Made by Justyn Dunford and Dawson Hampton
// Started March 21st, 2022
// Last modified on April 8th, 2022
// Note: Requires C++17 or later

#ifdef _WIN32

// Important: Windows will throw a hissy-fit 
// if you don't use its fancy fopen_s without this.
#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>

#endif // #ifdef _WIN32

#if defined(__unix__)

#include <unistd.h>

#endif // #if defined(__unix__)

#include <algorithm>
using std::find;

#include <cstdio>
using std::FILE;
using std::fclose;
using std::fopen;

#include <filesystem>
using std::filesystem::exists;
using std::filesystem::current_path;
using std::filesystem::remove;

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

// Takes every token of the vector and concatenates them, 
// each seperated by a " " character.
string concatenate(const vector<string>& vec, size_t begin)
{
	string str;
	size_t len = 0;

	for (size_t i = begin; i < vec.size(); ++i)
		len += vec[i].size() + 1;

	str.reserve(len);

	for (size_t i = begin; i < vec.size(); ++i)
		str += vec[i] + " ";

	return str;
}

// Creates the given file if it does not exist.
// Returns true on success.
// Returns false on error.
bool create_file(const string& path)
{
	if (exists(path))
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

// Removes the given file if it does not exist.
// Returns true on success.
// Returns false on error.
bool remove_file(const string& path)
{
	if (exists(path))
	{
		// remove will return false on error.
		if (!remove(path))
		{
			cout << "ERROR: Could not remove file: " << path << endl;
			return false;
		}
	}
	else
	{
		cout << "ERROR: Could not remove file: " << path << ": File does not exist." << endl;
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

// Sets the directory to the given directory.
// Prints an error if unsuccessful.
void set_directory(const string& dir)
{
	if (dir[0] == '~')
	{
		#ifdef _WIN32
			SetCurrentDirectoryA(getenv("USERPROFILE"));

			if (!SetCurrentDirectoryA(dir.c_str() + 1))
				cout << "ERROR: Directory not found." << endl;
		#endif // #ifdef _WIN32


	}
	else if (dir.find("..") != string::npos)
	{
		string subdir(dir);
		size_t loc = 0;

		while (loc != string::npos)
		{
			loc = subdir.find("..");

			if (loc != string::npos)
			{
				#ifdef _WIN32
					if (!SetCurrentDirectoryA(".."))
						cout << "ERROR: Directory not found." << endl;
					subdir = dir.substr(loc + 2);
				#endif // #ifdef _WIN32 
			}
		}

		#ifdef _WIN32
			if (!SetCurrentDirectoryA(subdir.c_str()))
				cout << "ERROR: Directory not found." << endl;
		#endif // #ifdef _WIN32 
	}
	else
	{
		#ifdef _WIN32
			if (!SetCurrentDirectoryA(dir.c_str()))
				cout << "ERROR: Directory not found." << endl;
		#endif // #ifdef _WIN32 
	}
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

		size_t token_count = get_tokens(tokens, line);

		if (tokens[0] == "help" || tokens[0] == "commands")
		{
			cout << endl;
			cout << "Available commands: " << endl;
			cout << "cd _directorypath_: Changes the current directory to the given directory." << endl;
			cout << "changedirectory _directorypath_: Changes the current directory to the given directory." << endl;
			cout << "createfile _filepath_: Creates a file in the current directory." << endl;
			cout << "echo _text_: Prints the given text." << endl;
			cout << "exit: Exits the terminal." << endl;
			cout << "list: Lists every file and directory in the current directory." << endl;
			cout << "ls: Lists every file and directory in the current directory." << endl;
			cout << "print: Prints the ID of the current process." << endl;
			cout << "quit: Exits the terminal." << endl;
			cout << "removefile _filepath_: Removes the given file." << endl;
			cout << "repeat _text_: Prints the given text." << endl;
			cout << "rm _filepath_: Removes the given file." << endl;
			cout << "touch _filepath_: Creates a file in the current directory." << endl;
			cout << "unlink _filepath_: Removes the given file." << endl;
			cout << endl;
		}
		else if (tokens[0] == "touch" || tokens[0] == "createfile")
		{
			if (tokens.size() > 1)
				create_file(tokens[1]);
			else
				cout << "ERROR: No file name given." << endl;
		}
		else if (tokens[0] == "rm" || tokens[0] == "unlink" || tokens[0] == "removefile")
		{
			if (tokens.size() > 1)
				remove_file(tokens[1]);
			else
				cout << "ERROR: No file name given." << endl;
		}
		else if (tokens[0] == "echo" || tokens[0] == "repeat")
		{
			if (tokens.size() == 1)
				cout << "ERROR: No text given." << endl;
			else
			{
				for (size_t i = 1; i < token_count; ++i)
					cout << tokens[i] << endl;
			}
		}
		else if (tokens[0] == "print")
		{
			#if defined(__unix__)
				cout << "Current process ID: " << getpid() << endl;
			#endif // #if defined(__unix__)

			#ifdef _WIN32
				cout << "Current process ID: " << GetCurrentProcessId() << endl;
			#endif // _WIN32
		}
		else if (tokens[0] == "cd" || tokens[0] == "changedirectory")
		{
			if (tokens.size() == 1)
				cout << "ERROR: No directory given." << endl;
			else if (tokens[1] == "..")
			{
				#if defined(__unix__)
					chdir("..");
				#endif // #if defined(__unix__)

				#ifdef _WIN32
					SetCurrentDirectoryA("..");
				#endif // #ifdef _WIN32 
			}
			else if (tokens[1] == "~")
			{
				#ifdef _WIN32
					SetCurrentDirectoryA(getenv("USERPROFILE"));
				#endif // #ifdef _WIN32
			}
			else
			{
				set_directory(concatenate(tokens, 1));
			}
		}
		else if (tokens[0] == "ls" || tokens[0] == "list")
		{
			#if defined(__unix__)
				system("ls");
			#endif // #if defined(__unix__)

			#ifdef _WIN32
				system("dir");
			#endif // _WIN32
		}
		else if (tokens[0] == "exit" || tokens[0] == "quit")
		{
			exit(0);
		}
		else
		{
			cout << "ERROR: Unknown command: " << tokens[0] << endl;
		}
	}
}

int main(int argc, char** argv)
{

	shell_loop();
	return 0;
}