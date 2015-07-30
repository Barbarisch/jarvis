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
bool thread_on = true;

Jarvis_net::Jarvis_net()
{
	//thread_on = true;
	//sockfd = 0;
	thread = 0;
}

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

	//listen for up to 5 simultaneous connections
	if(listen(sockfd,5) < 0) {
#ifdef DEBUG
		cout << "Listen failed. " << strerror(errno) << "\n";
#endif
		close(sockfd);
		return -1;
	}

	//set_nonblock(sockfd);

	data->sockfd = sockfd;

	cout << "sockfd: " << sockfd << '\n';
	cout << "data.sockfd: " << data->sockfd << '\n';
	
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

int Jarvis_net::end()
{
	void *ret = NULL;

	thread_on = false;
	if(thread != 0)
		pthread_join(thread, &ret);
	close(sockfd);
	return 0;
}

//thread function for accepting connections
void *net_listen(void *net_thread_arg)
{
	struct net_thread_data *data = NULL;
	struct sockaddr_in cli_addr;
	socklen_t clilen;
	int newsockfd;

	data = (struct net_thread_data *)net_thread_arg;

	clilen = sizeof(cli_addr);

	cout << "thread, data->sockfd: " << data->sockfd << '\n';
	cout << "thread, (*data).sockfd: " << (*data).sockfd << '\n';

	while(thread_on == true)
	{
		newsockfd = accept(data->sockfd, (struct sockaddr *) &(cli_addr), &(clilen));
		if(newsockfd < 0) {
			cout << "Accept error: " << strerror(errno) << "\n";
			break;
		}
	}
	
	close(newsockfd);

	if(data != NULL)
		delete data;
}

void set_nonblock(int socket) 
{
    int flags;
    flags = fcntl(socket,F_GETFL,0);
    assert(flags != -1);
    fcntl(socket, F_SETFL, flags | O_NONBLOCK);
	cout << "testing\n";
}
