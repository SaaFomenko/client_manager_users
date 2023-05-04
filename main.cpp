#include <iostream>
#include <fstream>
#include <pqxx/pqxx>
#include "lib/app_config/app_config.h"


int main()
{
	const std::string path_conf = "config.txt";

	try
	{
		const std::string connect_str = app_config::get_connect(path_conf);
	}
	catch(std::exception e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		pqxx::connection c(
		);
	} 
	catch (pqxx::sql_error e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}
