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
int FLAG = 1;

//function prototypes
int test_func()
{
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
