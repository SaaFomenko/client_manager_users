#include <iostream>
#include <fstream>
#include <pqxx/pqxx>
#include "lib/file_to_string/file_to_string.h"


int main()
{
	const std::string path_conf = "config.txt";
	const std::string path_create_table = "../create_tb.sql";
	std::string sql_str = "";
//	const std::string sql_str;
	std::string connect_str = "";

	try
	{
		//connect_str = app_config::get_connect(path_conf);
		connect_str = file_to_string(path_conf);
		sql_str = file_to_string(path_create_table);
	}
	catch(std::exception e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		pqxx::connection c(connect_str);

//		pqxx::work tx{ c };

		pqxx::work txb{ c };

		txb.exec0(sql_str);
		txb.commit();

		std::cout << "Tables will be create.\n";
/*
		for (
					auto [name, publication] : 
						tx.query<std::string, int>("SELECT name, publication FROM album")
				)
		{
			std::cout << "Альбом \"" << name << "\" был опубликован в: " << publication << " г.\n";
		}
*/

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
