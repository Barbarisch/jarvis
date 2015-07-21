#include <iostream>
#include <my_global.h>
#include <mysql.h>
#include <mysqld_error.h>

#define DBNAME "testdb"


class Jarvis_db 
{
	public:
		Jarvis_db();
		Jarvis_db(MYSQL *c);
		//~Jarvis_db();
		unsigned int init();
		unsigned int error();
		unsigned int close();
		unsigned int wipe(); //completely wipe database of all contents
		unsigned int setup(); //first time database creation
		unsigned int check();
		unsigned int add_command();
		MYSQL *get_con();
	//protected:
	private:
		MYSQL *con;
};


/* database functions */
//unsigned int db_setup(MYSQL **);
//unsigned int db_close(MYSQL *);
