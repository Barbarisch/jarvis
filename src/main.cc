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
struct thread_args {
	Jarvis_db *db;
};

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
void parse_input(string s, Jarvis_db *db);


//main function...
int main(int argc, char **argv)
{
	int ret = 0;
	void *join = NULL;
	Jarvis_db db;
	Jarvis_net net;
	string input = "";
	pthread_t console_input;
	struct thread_args args;

	cout << "Jarvis server\n\n";

	//register handle for ctrl-c
	signal(SIGINT, (sighandler_t)sigint_handler);

	//temp = db.init();
	//db.wipe();
	//db.setup();
	//db.close();

	//for(string elem : commands)
	//{
		//cout << elem << "\n";
	//}

	if(db.init() < 0)
		cout << "database init failure\n";

	db.setup();

	if(net.init() < 0)
		cout << "networking init failure\n";

	/*while(input.compare("exit") != 0)
	{
		cout << ">>";
		getline(cin, input);
		cout << "\n" << input << "\n";
	}*/
	
	args.db = &db;
	ret = pthread_create(&console_input, NULL, read_user_input, &args);
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

void *read_user_input(void *data)
{
	fd_set readset;
	timeval tv;
	int ret;
	string input = "";
	struct thread_args *args = (struct thread_args *)data;

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
				parse_input(input, args->db);
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

void parse_input(string s, Jarvis_db *db)
{
	string delimiter = " ";
	int temp;
	size_t pos = 0;
	string token;
	while ((pos = s.find(delimiter)) != string::npos) {
		token = s.substr(0, pos);
		temp = db->query_command(token);
		cout << "return: " << temp << "\n";
		//cout << token << "\n";
		s.erase(0, pos + delimiter.length());
	}
}
