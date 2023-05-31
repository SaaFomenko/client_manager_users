#include <iostream>
#include <exception>
#include <fstream>
#include <pqxx/pqxx>
#include "lib/my_file/my_file.h"


int main()
{
	const char* err_connect_file = "File not exist, or heve fail parametres.";
	const char* quest_create_file = "You do want create new file? (y/n): ";
	const char* path_connect = "connect_db.txt";
//	const std::string path_create_table = "../create_tb.sql";
//	std::string sql_str = "";
//	const std::string sql_str;
	const char* connect_str;

	try
	{
		MyFile file_read(path_connect);
		connect_str = file_read.file_to_chars(" <test> ");
	}
	catch(std::exception& e)
	{
		std::cout << e.what() << path_connect << '\n';
		std::cout << err_connect_file << '\n';
		std::cout << quest_create_file;		
	}

	try
	{
		pqxx::connection c(connect_str);

		pqxx::work tx{ c };

		const char* dbname = tx.query_value<const char*>("SELECT curent_database()");

		std::cout << "Wellcom you connected to database: " << dbname << "\n";

//		pqxx::work txb{ c };
/*
		txb.exec0(sql_str);
		txb.commit();

		std::cout << "Tables will be create.\n";

		for (
					auto [name, publication] : 
						tx.query<std::string, int>("SELECT name, publication FROM album")
				)
		{
			std::cout << "Альбом \"" << name << "\" был опубликован в: " << publication << " г.\n";
		}
*/

	} 
	catch (pqxx::failure e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}
