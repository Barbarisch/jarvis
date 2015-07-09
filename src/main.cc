#include <iostream>
#include <string>

#include "main.h"

using namespace std;

const string commands[] = {"test","light", "blah"};

int main(int argc, char **argv)
{
	//printf("testing\n");
	cout << "Jarvis\n\n";
	//cout << commands[0];
	//cout << "\n";

	for(string elem : commands)
	{
		cout << elem << "\n";
	}


	return 0;
}
