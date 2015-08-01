#include <iostream>

#include "main.h"
#include "database.h"

using namespace std;

//NOTE c++11 adjacent string literals are concatenated
// so this works: "DROP DATABASE "DBNAME

Jarvis_db::Jarvis_db()
{
	con = NULL;
}

Jarvis_db::Jarvis_db(MYSQL *c)
{
	con = c;
}

unsigned int Jarvis_db::error()
{
#ifdef DEBUG
		cout << mysql_error(con) << '\n';
#endif
		return mysql_errno(con);	
}

/**	init() - basic initialation of mysql object and connection
 *
 */
unsigned int Jarvis_db::init()
{
	unsigned int ret = 0;

	/* initialize msyql object */
	con = mysql_init(NULL);

	if(NULL == con)
		return error();

	/* connect to mysql database */
	if (mysql_real_connect(con, "localhost", "root", "password", DBNAME, 0, NULL, 0) == NULL) {
		if (mysql_real_connect(con, "localhost", "root", "password", NULL, 0, NULL, 0)) {
			if(mysql_query(con, "CREATE DATABASE "DBNAME) == NULL){
				mysql_close(con); // close non specific connection
				ret = init(); // recall init
			}			
		}
		else
			return error();
	}

	return ret;
}

unsigned int Jarvis_db::check()
{
	return 0;
}

/* clean creation of database */
unsigned int Jarvis_db::setup()
{
	unsigned int ret = 0;

	/* fill some tables */
	if((ret = mysql_query(con, "CREATE TABLE IF NOT EXISTS Commands(Name TEXT)")))
	{
		mysql_query(con, "INSERT INTO Commands VALUES('lights')");
		mysql_query(con, "INSERT INTO Commands VALUES('test')");
		mysql_query(con, "INSERT INTO Commands VALUES('version')");
	}

	if((ret = mysql_query(con, "CREATE TABLE IF NOT EXISTS Endpoints(ID TEXT)")))
	{

	}

	return ret;
}

unsigned int Jarvis_db::wipe()
{
	if(mysql_query(con, "DROP DATABASE IF EXISTS "DBNAME))
		return error();

	return 0;
}

unsigned int Jarvis_db::close()
{
	if(con != NULL)
		mysql_close(con);

	return 0;
}

unsigned int Jarvis_db::add_command()
{
	return 0;
}

MYSQL* Jarvis_db::get_con()
{
	return con;
}
