#include <iostream>
#include <strings.h>
#include <pthread.h>

#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "main.h"
#include "database.h"

using namespace std;

#define PORT 31337


class profile {

};

class endpoint {

};

//function prototypes
int test_func()
{
	return 0;
}

struct net_thread_data{
   int  thread_id;
};

void *net_listen(void *net_thread_arg)
{

}

int network_init()
{
	int sockfd, newsockfd;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	pthread_t net_thread;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
		cout << "Socket init failed\n";
		return -1;
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) { 
		cout << "Bind failed\n";
		return -1;
	}

	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	if (newsockfd < 0) {
		cout << "no connection made\n";
		return -1;
	}

	bzero(buffer,256);
	n = read(newsockfd,buffer,255);

	if (n < 0) 
		cout << "ERROR reading from socket\n";

	cout << "Here is the message: " << buffer << '\n';

	n = write(newsockfd,"I got your message",18);

	if (n < 0) 
		cout << "ERROR writing to socket\n";

	close(newsockfd);
	close(sockfd);

	return 0; 
}

//main function...
int main(int argc, char **argv)
{
	int temp = 0;
	Jarvis_db db;// = new Jarvis_db();

	//cout << "testing "DBNAME"\n";
	//MYSQL *con = NULL;

	//printf("testing\n");
	cout << "Jarvis\n\n";
	//cout << commands[0];
	//cout << "\n";

	temp = network_init();

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

	cout << "test returned: " << temp << "\n";


	return 0;
}
