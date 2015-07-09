#include <iostream>
#include <string>
#include <my_global.h>
#include <mysql.h>

#include "main.h"

using namespace std;

//supported commands
//TODO database should store these allowing for endpoints to register command types
const string commands[] = {"test","light", "blah"};


class profile {

};

class endpoint {

};

//function prototypes
int test_func()
{
	printf("MySQL client version: %s\n", mysql_get_client_info());

	MYSQL *con = mysql_init(NULL);

	if (con == NULL) 
	{
		printf("%s\n", mysql_error(con));
		return 1;
	}

	if (mysql_real_connect(con, "localhost", "root", "sisecure", NULL, 0, NULL, 0) == NULL) 
	{
		printf("%s\n", mysql_error(con));
		mysql_close(con);
		return 1;
	}  

	if (mysql_query(con, "CREATE DATABASE testdb")) 
	{
		printf("%s\n", mysql_error(con));
		mysql_close(con);
		return 1;
	}

	mysql_close(con);
}

//main function...
int main(int argc, char **argv)
{
	int temp = 0;

	//printf("testing\n");
	cout << "Jarvis\n\n";
	//cout << commands[0];
	//cout << "\n";

	temp = test_func();

	//for(string elem : commands)
	//{
		//cout << elem << "\n";
	//}

	cout << "test returned: " << temp << "\n";


	return 0;
}
