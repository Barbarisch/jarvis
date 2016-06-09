#ifndef NET_H
#define NET_H

#include <iostream>
#include <strings.h>

#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include <pthread.h>

#define MAX_CONN 512
#define PORT 31337

class Jarvis_net 
{
	public:
		Jarvis_net();
		int init();
		int end();
		//int send(int id, string msg);
	private:
		int sockfd;
		pthread_t thread;
		//int connfd[MAX_CONN];
};

#endif //NET_H
