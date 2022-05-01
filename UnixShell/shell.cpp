// shell.cpp
// Made by Justyn Dunford and Dawson Hampton
// Started March 21st, 2022
// Last modified on April 30th, 2022
// Note: Requires C++17 or later

// Thanks to TutorialsPoint for a code snippet. 
// Their work is commented where it is used!

// Thanks to brennan.io for a code snippet and a general guideline. 
// Their code snippet is commented where it is used!

#ifdef _WIN32

// Important: Windows will throw a hissy-fit 
// if you don't use its fancy fopen_s without this.
#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>

#endif // #ifdef _WIN32

#if defined(__unix__)

#include <pwd.h>
#include <unistd.h>
#include <sys/wait.h>

// For Signals.
#include <signal.h>

#endif // #if defined(__unix__)

#include <algorithm>
using std::find;
using std::remove;

#include <cstdio>
using std::FILE;
using std::fclose;
using std::fopen;

#include <filesystem>
using std::filesystem::exists;
using std::filesystem::current_path;
using std::filesystem::remove;

#include <functional>
using std::ref;

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <mutex>
using std::mutex;

#include <sstream>
using std::istringstream;

#include <string>
using std::size_t;
using std::string;
using std::getline;
using std::stoi;
using std::stod;
using std::to_string;

#include <thread>
using std::thread;

#include <vector>
using std::vector;

// Global variables.
// Stop is here to allow the sigHandler function to edit it 
// when it receives the signal from the child process.
bool stop = false;

// These are the locks to prevent race conditions.
mutex mutex_cout;
mutex mutex_file;

#ifdef _WIN32

// Class that streamlines the process of creating and
// running another process in windows.
class Win32Process
{
	int _pid;

	public:

	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	public:

	// Constructor.
	// Allocates memory for the process information and
	// sets the path of the process to exe_path.
	// Constructing the process starts it.
	Win32Process(const char* path = nullptr, char* args = nullptr)
	{
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		_pid = CreateProcessA(path, args, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	}

	// Destructor.
	// Closes the resources used by the new process.
	~Win32Process() noexcept
	{
		if (pi.hProcess)
		{
			CloseHandle(pi.hProcess);
			pi.hProcess = nullptr;
		}
		if (pi.hThread)
		{
			CloseHandle(pi.hThread);
			pi.hThread = nullptr;
		}
	}

	// Returns the ID of the new process.
	int getID() const noexcept
	{
		return _pid;
	}

	// Returns the handle of the process information.
	HANDLE processHandle() noexcept
	{
		return pi.hProcess;
	}
};

#endif // #ifdef _WIN32

// Cross-platform function to get the ID of the current process.
int get_pid()
{
	#if defined(__unix__)
		return getpid();
	#endif // #if defined(__unix__)

	#ifdef _WIN32
		return _getpid();
	#endif // _WIN32
}

// Prints every std::string contained in the given std::vector.
// Mostly for debug purposes.
void print(const vector<string>& vec)
{
	mutex_cout.lock();

	if (vec.empty())
	{
		cout << "{ }" << endl;
		mutex_cout.unlock();
		return;
	}

	cout << "{ ";

	for (size_t i = 0; i < vec.size() - 1; ++i)
		cout << vec[i] << ", ";

	cout << vec.back() << " }" << endl;

	mutex_cout.unlock();
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
	mutex_file.lock();

	if (exists(path))
	{
		mutex_cout.lock();
		cout << "ERROR: Cannot create file: " << path << ": File already exists." << endl;
		mutex_cout.unlock();

		mutex_file.unlock();
		return false;
	}

	// File does not exist, so create it.
	FILE* file = fopen(path.c_str(), "w");
	if (file)
		fclose(file);
	else
	{
		mutex_cout.lock();
		cout << "ERROR: Cannot create file: " << path << endl;
		mutex_cout.unlock();

		mutex_file.unlock();
		return false;
	}

	mutex_file.unlock();
	return true;
}

// Removes the given file if it exists.
// Returns true on success.
// Returns false on error.
bool remove_file(const string& path)
{
	mutex_file.lock();

	if (exists(path))
	{
		// remove will return false on error.
		if (!remove(path))
		{
			mutex_cout.lock();
			cout << "ERROR: Could not remove file: " << path << endl;
			mutex_cout.unlock();

			mutex_file.unlock();
			return false;
		}
	}
	else
	{
		mutex_cout.lock();
		cout << "ERROR: Could not remove file: " << path << ": File does not exist." << endl;
		mutex_cout.unlock();

		mutex_file.unlock();
		return false;
	}

	mutex_file.unlock();
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
	bool bad_char = false;
	istringstream stream(line);

	tokens.clear();

	while (stream >> token) 
	{
		for (size_t i = 0; i < token.length(); i++) 
		{
			if ((token[i] < '0' || ('9' < token[i] && token[i] < 'A') ||
				('Z' < token[i] && token[i] < 'a') || 'z' < token[i]) &&
				(token[i] != '-' && token[i] != '.' && token[i] != '/' && 
				token[i] != '_' && token[i] != '&' && token[i] != ';' && token[i] != '~'))
			{
				cout << "Unrecognized letter '" << token[i] << "' of word '" << token << "'!" << endl;
				bad_char = true;
				break;
			}
		}
		if (!bad_char) tokens.push_back(token);
	}

	return tokens.size();
}

// Prints the current directory followed by "> ".
void print_directory()
{
	mutex_cout.lock();

	#if defined(__unix__)
		cout << "\033[33m" << current_path() << "\033[0m";
		cout << "$ ";
	#endif // #if defined(__unix__)

	#ifdef _WIN32
		// This should change the background to black (0) and the text to yellow (6).
		// If this causes problems, delete it.
		system("Color 06");
		cout << current_path();
		cout << "> ";
	#endif // #ifdef _WIN32

	mutex_cout.unlock();
}

// Sets the directory to the given directory.
// Prints an error if unsuccessful.
void set_directory(const string& dir)
{
	// Lambda function.
	// Returns true on success.
	// Returns false on error.
	auto setdir = [](const char* dir)
	{
		#if defined (__unix__)
		return !chdir(dir);
		#endif // #if defined (__unix__)

		#ifdef _WIN32
		return SetCurrentDirectoryA(dir);
		#endif // #ifdef _WIN32 
	};

	if (dir[0] == '~')
	{
		#if defined (__unix__)
		chdir(getenv("HOME"));
		#endif // #if defined (__unix__)

		#ifdef _WIN32
		SetCurrentDirectoryA(getenv("USERPROFILE"));
		#endif // #ifdef _WIN32

		if (!setdir(dir.c_str() + 1))
		{
			mutex_cout.lock();
			cout << "ERROR: Directory not found." << endl;
			mutex_cout.unlock();

			return;
		}
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
				if (!setdir(".."))
				{
					mutex_cout.lock();
					cout << "ERROR: Directory not found." << endl;
					mutex_cout.unlock();

					return;
				}
			}
		}

		if (!setdir(subdir.c_str()))
		{
			mutex_cout.lock();
			cout << "ERROR: Directory not found." << endl;
			mutex_cout.unlock();

			return;
		}
	}
	else
	{
		if (!setdir(dir.c_str()))
		{
			mutex_cout.lock();
			cout << "ERROR: Directory not found." << endl;
			mutex_cout.unlock();
		}
	}
}

// Returns true if the given string starts with the given substring.
bool starts_with(const string& str, const string& substr)
{
	if (str.size() < substr.size())
		return false;

	return str.substr(0, substr.size()) == substr;
}

// This function interprets commands.
// This function has to be by itself so that ; and & can delimit.
void commandFunction(vector<string>& tokens, size_t token_count, int ppid, int pid) 
{
	if (tokens.size() == 0)
	{
		// Do nothing.
	}
	else if (tokens[0] == "help" || tokens[0] == "commands")
	{
		mutex_cout.lock();
		cout << endl;
		cout << "Available commands: " << endl;
		cout << "cd _directorypath_: Changes the current directory to the given directory." << endl;
		cout << "changedirectory _directorypath_: Changes the current directory to the given directory." << endl;
		cout << "createfile _filepath_: Creates a file in the current directory." << endl;
		cout << "date: Prints the current time and date." << endl;
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
		cout << "who: Lists the current user." << endl;
		cout << endl;
		mutex_cout.unlock();
	}
	else if (tokens[0] == "touch" || tokens[0] == "createfile")
	{
		if (tokens.size() > 1) 
		{
			create_file(tokens[1]);
		}
		else 
		{
			mutex_cout.lock();
			cout << "ERROR: No file name given." << endl;
			mutex_cout.unlock();
		}
	}
	else if (tokens[0] == "rm" || tokens[0] == "unlink" || tokens[0] == "removefile")
	{
		if (tokens.size() > 1) 
		{
			remove_file(tokens[1]);
		}
		else 
		{
			mutex_cout.lock();
			cout << "ERROR: No file name given." << endl;
			mutex_cout.unlock();
		}
	}
	else if (tokens[0] == "echo" || tokens[0] == "repeat")
	{
		if (tokens.size() == 1) 
		{
			mutex_cout.lock();
			cout << "ERROR: No text given." << endl;
			mutex_cout.unlock();
		}
		else
		{
			mutex_cout.lock();

			for (size_t i = 1; i < token_count; ++i)
				cout << tokens[i] << endl;

			mutex_cout.unlock();
		}
	}
	else if (tokens[0] == "print")
	{
		mutex_cout.lock();
		cout << "Current process ID: " << get_pid() << endl;
		mutex_cout.unlock();
	}
	else if (tokens[0] == "cd" || tokens[0] == "changedirectory")
	{
		if (tokens.size() == 1) 
		{
			mutex_cout.lock();
			cout << "ERROR: No directory given." << endl;
			mutex_cout.unlock();
		}
		else if (tokens[1] == "..")
		{
			mutex_file.lock();

			#if defined(__unix__)
				chdir("..");
			#endif // #if defined(__unix__)

			#ifdef _WIN32
				SetCurrentDirectoryA("..");
			#endif // #ifdef _WIN32 

			mutex_file.unlock();
		}
		else if (tokens[1] == "~")
		{
			#ifdef _WIN32
				mutex_file.lock();
				SetCurrentDirectoryA(getenv("USERPROFILE"));
				mutex_file.unlock();
			#endif // #ifdef _WIN32

			#if defined(__unix__)

			// The following code snippet is from https://www.tutorialspoint.com/How-to-execute-a-command-and-get-the-output-of-command-within-Cplusplus-using-POSIX
			// Thank you, TutorialsPoint! 
			// This is useful for getting the username of the user to 
			// be directed to the home directory.
			// The mutex mutex_cout was made by the writers of this program 
			// (Not TutorialsPoint) to fight race conditions.

			char buffer[128];
			string result;

			// Open pipe to file.
			FILE* pipe = popen("whoami", "r");
			if (!pipe) 
			{
				mutex_cout.lock();
				cout << "popen failed!" << endl;
				mutex_cout.unlock();
			}

			// Read till end of process:
			while (!feof(pipe)) 
			{
			   // Use buffer to read and add to result.
				if (fgets(buffer, 128, pipe) != NULL)
					result += buffer;
			}

			pclose(pipe);

			// This is the end of the TutorialsPoint function.

			result.erase(remove(result.begin(), result.end(), '\n'), result.end());

			string newDirec = "/home/" + result + "/";
			mutex_file.lock();
			chdir(newDirec.c_str());
			mutex_file.unlock();

			#endif // #ifdef _unix
		}
		else
		{
			mutex_file.lock();

			#ifdef _WIN32
				set_directory(concatenate(tokens, 1));
			#endif // #ifdef _WIN32

			#if defined(__unix__)

				if (tokens[1][0] == '/') // Absolute directory.
					chdir(tokens[1].c_str());
				else 
				{  
					// Relative Directory/
					string currDir = current_path();
					string newDir = currDir + "/" + tokens[1];
					chdir(newDir.c_str());
				}

			#endif // #ifdef _unix
			
			mutex_file.unlock();
		}
	}
	else if (tokens[0] == "ls" || tokens[0] == "list")
	{
		mutex_file.lock();
		mutex_cout.lock();

		#if defined(__unix__)
			system("ls");
		#endif // #if defined(__unix__)

		#ifdef _WIN32
			system("dir");
		#endif // _WIN32

		mutex_cout.unlock();
		mutex_file.unlock();
	}
	else if (tokens[0] == "exit" || tokens[0] == "quit")
	{
		#if defined (__unix__)
			kill(ppid, SIGUSR1);
		#endif // #if defined (__unix__)

		exit(0);
	}
	else if (starts_with(tokens[0], "./")) // The token starts with ./ and is an executable file!
	{
		string path = tokens[0].substr(2);

		if (exists(path))
		{
			#if defined (__unix__)

			vector<char*> commands;
			for (int i = 0; i < tokens.size(); i++)
			{
				commands.push_back(&tokens[i][0]);
			}

			commands.push_back(NULL);
			char** theCommand = &commands[0];

			pid_t pid, wpid;
			int status;

			pid = fork();

			// The following code is taken from https://brennan.io/2015/01/16/write-a-shell-in-c/
			if (pid == 0) // Child process
			{
				if (execvp(theCommand[0], theCommand) == -1)
				{
					perror("lsh");
				}

				exit(EXIT_FAILURE);
			}
			else if (pid < 0) // Error forking.
			{
				mutex_cout.lock();
				cout << "Error forking" << endl;
				mutex_cout.unlock();
				perror("lsh");
			}
			else // Parent process
			{
				do
				{
					wpid = waitpid(pid, &status, WUNTRACED);
				} while (!WIFEXITED(status) && !WIFSIGNALED(status));
			}

			#endif // #if defined (__unix__)

			#ifdef _WIN32

			string args;

			for (size_t i = 1; i < tokens.size(); ++i)
				args += tokens[i] + " ";

			Win32Process proc(tokens[0].substr(2).c_str());

			if (proc.processHandle())
			{
				// Wait until the new process exits.
				WaitForSingleObject(proc.processHandle(), INFINITE);
			}
			else
			{
				mutex_cout.lock();
				cout << "ERROR: Could not start new process." << endl;
				mutex_cout.unlock();
			}

			#endif // #ifdef _WIN32
		}
	}
	else if (tokens[0] == "who")
	{
		#if defined(__unix__)
		system("who");
		#endif // #if defined(__unix__)

		#ifdef _WIN32

		DWORD username_len = 256;
		string username(username_len, ' ');
		GetUserNameA(username.data(), &username_len);
		username.resize(username_len);

		mutex_cout.lock();
		cout << username << endl;
		mutex_cout.unlock();

		#endif // #ifdef _WIN32
	}
	else if (tokens[0] == "date")
	{
		#if defined(__unix__)
		system("date");
		#endif // #if defined(__unix__)

		#ifdef _WIN32
		system("date /t");
		#endif // #ifdef _WIN32
	}
	else
	{
		string unknownCommand;
		for (size_t i = 0; i < tokens.size(); i++) unknownCommand = unknownCommand + " " + tokens[i];

		mutex_cout.lock();
		cout << "ERROR: Command '" << tokens[0] << "' not recognized by this shell.  Passing it off to bash..." << endl;
		mutex_cout.unlock();

		system(unknownCommand.c_str());
	}

	#if defined(__unix__)
	if (pid != ppid) exit(0);
	#endif // #if defined(__unix__)
}

// 
void checkForDelimit(string& line, vector<string>& tokens, size_t token_count, int pid) 
{
	if (line.find(';') == string::npos && line.find('&') == string::npos)
	{
		commandFunction(tokens, tokens.size(), get_pid(), get_pid());
		return;
	}
	else
	{
		vector<vector<string>> broken;
		vector<string> line;
		size_t delimitCount = 0;
		string holder;
		vector<string> delimits;
		size_t childProcessCount = 0;
		int new_pid;

		#ifdef _WIN32

		vector<thread> threads;

		#endif // #ifdef _WIN32

		for (size_t i = 0; i < token_count; i++) 
		{
			holder = tokens[i];
			if (holder == ";" || holder == "&") 
			{
				delimitCount++;
				delimits.push_back(holder);
				broken.push_back(line);
				line.clear();
			}
			else line.push_back(holder);
		}

		broken.push_back(line);
		delimits.push_back(";");
		delimitCount++;

		for (size_t i = 0; i < delimitCount; i++)
		{
			#if defined(__unix__)

			if (delimits[i] == "&")
			{
				childProcessCount++;

				fork();
				new_pid = getpid();

				if (new_pid != pid) // Child process
					commandFunction(broken[i], broken[i].size(), pid, new_pid);
			}
			else
				commandFunction(broken[i], broken[i].size(), pid, pid);

			#endif // #if defined(__unix__)

			#ifdef _WIN32

			if (delimits[i] == "&")
			{
				childProcessCount++;
				threads.push_back(thread(commandFunction, ref(broken[i]), broken[i].size(), pid, new_pid));
			}
			else
				commandFunction(broken[i], broken[i].size(), pid, pid);

			#endif // _WIN32
		}

		#if defined(__unix__)

		// Wait for all child processes to finish.
		for (size_t i = 0; i < childProcessCount; i++) wait(NULL);

		#endif // #if defined(__unix__)

		#ifdef _WIN32

		for (size_t i = 0; i < threads.size(); ++i)
			threads[i].join();

		#endif // #ifdef _WIN32
	}
}

// This function allows the parent process to receive the stop signal from the children.
void sigHandler(int number) 
{
	#if defined (__unix__)

		if (number = SIGUSR1)
		{
			cout << "Stop detected on pid " << getpid() << endl;
			stop = true;
		}

	#endif // #if defined (__unix__)
}

// Main loop for the shell.
// Currently only loops once.
void shell_loop()
{
	string line(100, ' ');
	vector<string> tokens;

	#if defined (__unix__)

		if (signal(SIGUSR1, sigHandler) == SIG_ERR)
		{
			mutex_cout.lock();
			cout << "Can't catch SIGUSR1!" << endl;
			mutex_cout.unlock();
		}

	#endif // #if defined (__unix__)

	while (!stop)
	{
		print_directory();

		getline(cin, line);

		if (line.size() > 100)
		{
			mutex_cout.lock();
			cout << "ERROR: Cannot parse line greater than 100 characters." << endl;
			line.resize(100);
			mutex_cout.unlock();
		}

		size_t token_count = get_tokens(tokens, line);

		if (token_count == 0) 
		{
			mutex_cout.lock();
			cout << "No words were recognized!  Please try again." << endl;
			mutex_cout.unlock();
			continue;
		}

		checkForDelimit(line, tokens, token_count, get_pid());
	}
}

int main(int argc, char** argv)
{
	shell_loop();

	#if defined(__unix__)

	// End-of-program cleanup. 	
	// Make sure there are no child processes.
	while (wait(NULL) > 0); // wait(NULL) returns -1 if there are no child processes to wait for.

	#endif // #if defined(__unix__)

	return 0;
}