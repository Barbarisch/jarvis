#include <iostream>

#include "net.h"
#include "main.h"

//private function prototypes
void *net_listen2(void *net_thread_arg);
void set_nonblock(int socket);

//thread data structure
struct net_thread_data{
	int *sockfd;
};

//globals
bool thread_on = true;

Jarvis_net::Jarvis_net()
{
	//thread_on = true;
	//sockfd = 0;
}

int Jarvis_net::init()
{
	int ret = 0;
	struct sockaddr_in serv_addr;
	struct net_thread_data data;

	//create file descriptor for socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
#ifdef DEBUG
		cout << "Socket init failed\n";
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
		cout << "Bind failed\n";
#endif
		return -1;
	}

	//listen for up to 5 simultaneous connections
	if(listen(sockfd,5) < 0) {
#ifdef DEBUG
		cout << "Listen failed\n";
#endif
		close(sockfd);
		return -1;
	}

	set_nonblock(sockfd);

	data.sockfd = &sockfd;
	ret = pthread_create(&thread, NULL, net_listen2, &data);

	return 0; 
}

int Jarvis_net::end()
{
	void *ret = NULL;

	thread_on = false;
	pthread_join(thread, &ret);
	close(sockfd);
	return 0;
}

//thread function for accepting connections
void *net_listen2(void *net_thread_arg)
{
	struct net_thread_data *data;
	struct sockaddr_in cli_addr;
	socklen_t clilen;
	int newsockfd;

	data = (struct net_thread_data *)net_thread_arg;

	clilen = sizeof(cli_addr);

	while(thread_on == true)
	{
		newsockfd = accept(*(data->sockfd), (struct sockaddr *) &(cli_addr), &(clilen));
	}
	
	close(newsockfd);
}
