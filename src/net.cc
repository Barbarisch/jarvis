#include <iostream>
#include <errno.h>
#include <string.h>
//#include <pthread> //inside net.h
#include <string>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <assert.h>
#include <stdio.h>

#include "net.h"
#include "main.h"

using namespace std;

//private function prototypes
void *net_listen(void *net_thread_arg);
void set_nonblock(int socket);

//thread data structure
struct net_thread_data{
	int sockfd;
};

//globals
bool net_thread_on = true;

//net object constructor
Jarvis_net::Jarvis_net()
{
	thread = 0;
}

/**
 *	Jarvis_net::init() - initialize networking for jarvis
 *	
 *	Creates main server socket for listening for incoming
 *	connections. Set socket to non-blocking and start
 *	thread for listening logic. 
 *	*NOTE* thread data struct must be freed in thread function
 *	*NOTE* thread is stopped by setting global net_thread_on bool
 *
 *	Return: 0 on success, -1 on failure
 */
int Jarvis_net::init()
{
	int ret = 0;
	struct sockaddr_in serv_addr;
	struct net_thread_data *data = new struct net_thread_data;

	//create file descriptor for socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
#ifdef DEBUG
		cout << "Socket init failed. " << strerror(errno) << "\n";
#endif
		return -1;
	}

	//fill sockaddr struct with server info
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT);

	//bind socket to port
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
#ifdef DEBUG
		cout << "Bind failed. " << strerror(errno) << "\n";
#endif
		return -1;
	}

	//listen for connections
	if(listen(sockfd,5) < 0) {
#ifdef DEBUG
		cout << "Listen failed. " << strerror(errno) << "\n";
#endif
		close(sockfd);
		return -1;
	}

	set_nonblock(sockfd);

	data->sockfd = sockfd;
	
	//start thread for accepting connections
	ret = pthread_create(&thread, NULL, net_listen, (void *)data);
	
	if(ret != 0) {
#ifdef DEBUG
		cout << "pthread_create failed. " << strerror(errno) << "\n";
#endif
		close(sockfd);
		return -1;
	}

	return 0; 
}

/**
 *	Jarvis_net::end() - cleanup networking stuff
 *
 *	Signal threads to stop and closes main socket.
 */
int Jarvis_net::end()
{
	void *ret = NULL;

	net_thread_on = false;
	if(thread != 0)
		pthread_join(thread, &ret);
	close(sockfd);
	return 0;
}

/**
 *	net_listen() - thread function for accepting connections
 *
 *	Uses select function for creating a non-blocking accept
 *	mechanism.
 *	TODO add threads for individual connections
 */
void *net_listen(void *net_thread_arg)
{
	struct net_thread_data *data = NULL;
	struct sockaddr_in cli_addr;
	socklen_t clilen;
	int newsockfd;
	fd_set readset;
	timeval tv;
	int ret;

	data = (struct net_thread_data *)net_thread_arg;

	clilen = sizeof(cli_addr);

	while(net_thread_on == true)
	{
		FD_ZERO(&readset);
		FD_SET(data->sockfd, &readset);

		tv.tv_sec = 5;
		tv.tv_usec = 0;

		ret = select(data->sockfd+1, &readset, NULL, NULL, &tv);
		
		if(ret > 0)
		{
			if(FD_ISSET(data->sockfd, &readset))
			{
				newsockfd = accept(data->sockfd, (struct sockaddr *) &(cli_addr), &(clilen));
				if(newsockfd < 0) {
#ifdef DEBUG
					cout << "Accept error: " << strerror(errno) << "\n";
#endif
					break;
				}
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
	
	//TODO figure out how to close all open sockets
	//close(newsockfd);

	if(data != NULL)
		delete data;
}

//set a socket to non-blocking
void set_nonblock(int socket) 
{
    int flags;
    flags = fcntl(socket,F_GETFL,0);
    assert(flags != -1);
    fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}
