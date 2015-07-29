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

#define PORT 31337

//globals
int FLAG = 1;
int listen_flag = 0;
int read_flag = 0;

//function prototypes
int test_func()
{
	return 0;
}

struct net_thread_data{
	int  thread_id;
	int sockfd;
	int newsockfd;
	socklen_t clilen;
	struct sockaddr_in cli_addr;
};

struct conn_thread_data {
	int thread_id;
	int sockfd;
};

void *conn_thread(void *conn_thread_arg)
{

}

void *net_listen(void *net_thread_arg)
{
	struct sockaddr_in cli_addr;
	struct net_thread_data *test;

	test = (struct net_thread_data *)net_thread_arg;

	while(listen_flag == 0)
	{
		test->clilen = sizeof(cli_addr);
		test->newsockfd = accept(test->sockfd, (struct sockaddr *) &(test->cli_addr), &(test->clilen));
	}
}

void set_nonblock(int socket) 
{
    int flags;
    flags = fcntl(socket,F_GETFL,0);
    assert(flags != -1);
    fcntl(socket, F_SETFL, flags | O_NONBLOCK);
	cout << "testing\n";
}

int network_init()
{
	int sockfd, newsockfd;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	pthread_t net_thread;
	struct net_thread_data test;

	test.sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (test.sockfd < 0) {
		cout << "Socket init failed\n";
		return -1;
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT);

	if (bind(test.sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) { 
		cout << "Bind failed\n";
		return -1;
	}

	listen(test.sockfd,5);
	set_nonblock(test.sockfd);
	//clilen = sizeof(cli_addr);
	//set_nonblock(sockfd);
	//newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	//if (newsockfd < 0) {
	//	cout << "no connection made\n";
	//	return -1;
	//}
	int blah;
	blah = pthread_create(&net_thread, NULL, net_listen, &test);
	cout << "blah: " << blah << '\n';

	while(FLAG == 1)
	{
		cout << "getting input: ";
		cin >> FLAG;
		cout << "\ngot input: " << FLAG << '\n';
		listen_flag = 1;
	}


	void *ret = NULL;
	pthread_join(net_thread, &ret);

	cout << "ret: " << ret << '\n';

	close(test.newsockfd);
	close(test.sockfd);

	return 0; 
}

//main function...
int main(int argc, char **argv)
{
	int temp = 0;
	Jarvis_db db;// = new Jarvis_db();
	Jarvis_net net;

	//cout << "testing "DBNAME"\n";
	//MYSQL *con = NULL;

	//printf("testing\n");
	cout << "Jarvis\n\n";
	//cout << commands[0];
	//cout << "\n";

	//temp = network_init();

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

	temp = net.init();
	cout << "temp = " << temp << '\n';

	while(FLAG == 1)
	{
		cout << "getting input: ";
		cin >> FLAG;
		cout << "\ngot input: " << FLAG << '\n';
	}

	net.end();

	cout << "test returned: " << temp << "\n";


	return 0;
}
