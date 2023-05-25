#include <iostream>
#include <fstream>
#include <pqxx/pqxx>
#include "lib/app_config/app_config.h"


int main()
{
	const std::string path_conf = "config.txt";
	std::string connect_str = "";

	try
	{
		connect_str = app_config::get_connect(path_conf);
	}
	catch(std::exception e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		pqxx::connection c(connect_str);

		pqxx::work tx{ c };

		for (
					auto [name, publication] : 
						tx.query<std::string, int>("SELECT name, publication FROM album")
				)
		{
			std::cout << "Альбом \"" << name << "\" был опубликован в: " << publication << " г.\n";
		}
	} 
	catch (pqxx::sql_error e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}
