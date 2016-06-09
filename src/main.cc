#include <iostream>
#include <string>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>   //signal(3)
#include <sys/stat.h> //umask(3)
#include <syslog.h>   //syslog(3), openlog(3), closelog(3)
#include <stdio.h>
#include <stdlib.h> //exit

#include "main.h"
#include "database.h"
#include "net.h"
#include "parser.h"

using namespace std;

//globals
bool thread_on = true;
bool daemon_on = true;

//other resources
struct thread_args {
	Jarvis_db *db;
};

//function prototypes
void start_cli();
void *read_user_input(void *args);
void parse_input(string s, Jarvis_db *db);
void show_help();
int daemonize();
void signal_handler(sig_t s);
void daemon_signal_handler(int sig);

//main function...
int main(int argc, char **argv)
{
	int c, option_index = 0;
	bool make_daemon = false;

	cout << "Jarvis server\n\n";

	//parsing command line arguments
	if(argc < 2)
	{
		show_help();
		return 0;
	}

	while (1)
	{
		static struct option long_options[] =
		{
			{"verbose",		no_argument, 0, 'v'},
			{"help",		no_argument, 0, 'h'},
			{"daemonize",	no_argument, 0, 'd'},
			{"cli",			no_argument, 0, 'c'},
			{0, 0, 0, 0}
		};
		
		option_index = 0;

		c = getopt_long (argc, argv, "hdvcab", long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
			break;

		switch (c)
		{
		case 'a':
			cout << "testing a \n";
			break;
		case 'b':
			cout << "testing b \n";
			sleep(7);
			break;
		case 'h':
			show_help();
			break;
		case 'c':
			cout << "option -(-c)li\n";
			break;
		case 'd':
			cout << "option -(-d)aemonize\n";
			make_daemon = true;
			break;
		case 'v':
			cout << "option -(-v)erbose does nothing for now\n";
		case '?':
			show_help();
			break;
		default:
			abort();
		}
	}

	if(make_daemon)
		daemonize();
	else
		start_cli();

	return 0;
}

void start_cli()
{
	int ret = 0;
	void *join = NULL;
	Jarvis_db db;
	Jarvis_net net;
	string input = "";
	pthread_t console_input;
	struct thread_args args;

	//register handle for ctrl-c
	signal(SIGINT, (sighandler_t)signal_handler);

	//temp = db.init();
	//db.wipe();
	//db.setup();
	//db.close();

	//for(string elem : commands)
	//{
		//cout << elem << "\n";
	//}

	if(db.init() < 0)
		cerr << "database init failure\n";

	db.setup();

	if(net.init() < 0)
		cerr << "networking init failure\n";

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
}

void show_help()
{
	cout << "Usage: jarvis [OPTION]\n";
	cout << "\t-(-h)elp     \t\n";
	cout << "\t-(-c)li      \t\n";
	cout << "\t-(-d)aemonize\t\n";
	cout << "\t-(-v)erbose  \t\n";
	cout << "\nTry `jarvis -(-h)elp' for more options.\n";
}

int daemonize()
{
	pid_t daemon;

	const char *path = "/";
	const char *name = "jarvisd";
	const char *file = "/dev/null";
    
	//fork
    if((daemon = fork()) < 0) {
        cerr << "error: failed fork\n";
        exit(EXIT_FAILURE);
    }
	
	//signal parent process to shutdown
    if(daemon > 0) { //parent
        exit(EXIT_SUCCESS);
    }

	//daemon becomes session leader
    if(setsid() < 0) {
        cerr << "error: failed setsid\n";
        exit(EXIT_FAILURE);
    }

    //catch/ignore signals
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
	signal(SIGINT, (sighandler_t)daemon_signal_handler);
	signal(SIGTERM, (sighandler_t)daemon_signal_handler);

    //fork again
    if ((daemon = fork()) < 0) {
        cerr << "error: failed fork\n";
        exit(EXIT_FAILURE);
    }

	//signal parent to terminate
    if(daemon > 0) {
        exit(EXIT_SUCCESS);
    }

    //set file permissions and change directory
    umask(0);
    chdir(path);

    //Close all open file descriptors
    int fd;
    for(fd=sysconf(_SC_OPEN_MAX); fd>0; --fd) {
        close(fd);
    }

    //reopen stdin, stdout, stderr
    stdin=fopen(file, "r");   //fd=0
    stdout=fopen(file, "w+");  //fd=1
    stderr=fopen(file, "w+");  //fd=2

    //open syslog
    openlog(name, LOG_PID, LOG_DAEMON);

	syslog(LOG_NOTICE, "jarvisd started.");

	while(daemon_on) {
		sleep(3);
	}

	syslog(LOG_NOTICE, "jarvisd terminated.");

    return(0);
}

void *read_user_input(void *data)
{
	fd_set readset;
	timeval tv;
	int ret;
	string input = "";
	size_t len = 0;
	char *line = NULL;
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
				//getline(cin, input);
				if(getline(&line, (size_t *)&len, stdin) != 1) {
					//cout << input << "\n";
					//parse_input(input, args->db);
					parse_sentence(line);
					//if(input.compare("exit") == 0) {
					if(!strncmp(line, "exit", 4)) {
						break;
					}
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
			//cout << "select error: " << strerror(errno) << "\n";
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

void signal_handler(sig_t s)
{
	thread_on = false;
}

void daemon_signal_handler(int sig)
{
	syslog(LOG_NOTICE, "jarvisd signal handler.");
	switch(sig)
	{
	case SIGINT:
	case SIGTERM:
		syslog(LOG_NOTICE, "jarvisd terminating.");
		daemon_on = false;
		break;
	}
}
