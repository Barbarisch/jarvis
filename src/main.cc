#include <iostream>
#include <string>
#include <pthread.h>

#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "main.h"
#include "database.h"
#include "net.h"

using namespace std;

//globals
bool thread_on = true;

//other resources

//function prototypes
int test_func()
{
	return 0;
}

void sigint_handler(sig_t s)
{
	thread_on = false;
}

void *read_user_input(void *args);
void string_split(string s);


//main function...
int main(int argc, char **argv)
{
	int ret = 0;
	void *join = NULL;
	Jarvis_db db;
	Jarvis_net net;
	string input = "";
	pthread_t console_input;

	cout << "Jarvis server\n\n";

	//register handle for ctrl-c
	signal(SIGINT, (sighandler_t)sigint_handler);


	//temp = test_func();
	//temp = db_setup(&con);
	//db_close(con);
	//temp = db.init();
	//db.wipe();
	//db.setup();
	//db.close();

	//for(string elem : commands)
	//{
		//cout << elem << "\n";
	//}

	if(net.init() < 0)
		cout << "networking init failure\n";

	/*while(input.compare("exit") != 0)
	{
		cout << ">>";
		getline(cin, input);
		cout << "\n" << input << "\n";
	}*/
	
	ret = pthread_create(&console_input, NULL, read_user_input, NULL);
	if(ret == 0) {
#ifdef DEBUG
		//cout << "thread started\n";
#endif
		pthread_join(console_input, &join);
	}

	//cleanput
	cout << "cleaning up...\n";
	net.end(); //blocking call

	return 0;
}

void *read_user_input(void *args)
{
	fd_set readset;
	timeval tv;
	int ret;
	string input = "";

	cout << ">>";
	fflush(stdout);

	while(thread_on == true)
	{
		FD_ZERO(&readset);
		FD_SET(STDIN_FILENO, &readset);

		tv.tv_sec = 0;
		tv.tv_usec = 500000;

		ret = select(STDIN_FILENO+1, &readset, NULL, NULL, &tv);
		
		if(ret > 0)
		{
			if(FD_ISSET(STDIN_FILENO, &readset))
			{
				getline(cin, input);
				//cout << input << "\n";
				string_split(input);
				if(input.compare("exit") == 0) {
					break;
				}
				cout << ">>";
				fflush(stdout);
			}
		}
		else if(ret == 0) {
#ifdef DEBUG
			//cout << "select timeout: " << strerror(errno) << "\n";
#endif
		}
		else {
#ifdef DEBUG
			cout << "select error: " << strerror(errno) << "\n";
#endif
			break;
		}
	}
}

void string_split(string s)
{
	string delimiter = " ";

	size_t pos = 0;
	string token;
	while ((pos = s.find(delimiter)) != string::npos) {
		token = s.substr(0, pos);
		cout << token << "\n";
		s.erase(0, pos + delimiter.length());
	}
}
