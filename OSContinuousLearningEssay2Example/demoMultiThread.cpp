// OSContinuousLearningEssay2Example
// demoMultiThread.cpp
// Created on 2022-02-06 by Justyn Durnford
// Last modified on 2022-02-06 by Justyn Durnford
// Program to demonstrate the use of multithreading in C++

#include <iostream>
using std::cout;

#include <thread>
using std::thread;

class DummyClass
{
	int _id;

	public:

	DummyClass(int id)
	{
		_id = id;

		switch (_id)
		{
			case 1: cout << "Thread 1 has called DummyClass::DummyClass()\n"; break;
			case 2: cout << "Thread 2 has called DummyClass::DummyClass()\n"; break;
			case 3: cout << "Thread 3 has called DummyClass::DummyClass()\n"; break;
			case 4: cout << "Thread 4 has called DummyClass::DummyClass()\n"; break;

			default: break;
		}
	}

	void printMessage()
	{
		switch (_id)
		{
			case 1: cout << "Thread 1 has called DummyClass::printMessage()\n"; break;
			case 2: cout << "Thread 2 has called DummyClass::printMessage()\n"; break;
			case 3: cout << "Thread 3 has called DummyClass::printMessage()\n"; break;
			case 4: cout << "Thread 4 has called DummyClass::printMessage()\n"; break;

			default: break;
		}
	}

	~DummyClass()
	{
		switch (_id)
		{
			case 1: cout << "Thread 1 has called DummyClass::~DummyClass()\n"; break;
			case 2: cout << "Thread 2 has called DummyClass::~DummyClass()\n"; break;
			case 3: cout << "Thread 3 has called DummyClass::~DummyClass()\n"; break;
			case 4: cout << "Thread 4 has called DummyClass::~DummyClass()\n"; break;

			default: break;
		}
	}
};

void funct(int thread_num)
{
	switch (thread_num)
	{
		case 1: cout << "Thread 1 has called funct()\n"; break;
		case 2: cout << "Thread 2 has called funct()\n"; break;
		case 3: cout << "Thread 3 has called funct()\n"; break;
		case 4: cout << "Thread 4 has called funct()\n"; break;

		default: break;
	}

	DummyClass obj(thread_num);
	obj.printMessage();
}

int main()
{
	thread thr1(funct, 1);
	thread thr2(funct, 2);
	thread thr3(funct, 3);
	thread thr4(funct, 4);

	thr1.join();
	thr2.join();
	thr3.join();
	thr4.join();

	return 0;
}