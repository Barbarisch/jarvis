#include <iostream>
#include <string>

#include "main.h"
#include "database.h"

using namespace std;


class profile {

};

class endpoint {

};

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

	//cout << "testing "DBNAME"\n";
	//MYSQL *con = NULL;

	//printf("testing\n");
	cout << "Jarvis\n\n";
	//cout << commands[0];
	//cout << "\n";

	//temp = test_func();
	//temp = db_setup(&con);
	//db_close(con);
	temp = db.init();
	//db.wipe();
	db.setup();
	db.close();

	//for(string elem : commands)
	//{
		//cout << elem << "\n";
	//}

	cout << "test returned: " << temp << "\n";


	return 0;
}
