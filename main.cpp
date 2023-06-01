#include <unistd.h>
#include <iostream>
#include <exception>
#include <fstream>
#include <pqxx/pqxx>
#include "lib/my_file/my_file.h"


int main()
{
	const char* path_connect = "connect_db.txt";
	const char* err_connect_file = "File not exist, or heve fail parametres.";
	const char* quest_create_file = "You do want create new file? (y/n): ";
	const char* enter_connect_params = "Enter params for connect DB:\n";

	const unsigned int size_connect_params = 64; 
	const unsigned int buff_size = 1024; 
	const char* local_host = "localhost";
	const char* default_port = "5432";
	const char* host_quest = "host=(default: localhost): ";
	const char* host_param = "host=";
	const char* port_quest = "port=(default: 5432): ";
	const char* port_param = "port=";
	const char* name_quest = "dbname=";
	const char* user_quest = "user=";
	const char* password_quest = "password=";
//	const std::string path_create_table = "../create_tb.sql";
//	std::string sql_str = "";
//	const std::string sql_str;
	const char* connect_str;
	char temp[buff_size] = "host=";

	try
	{
		MyFile file_read(path_connect);
		connect_str = file_read.to_str();
	}
	catch(std::exception& e)
	{
		std::cout << e.what() << path_connect << '\n';
		std::cout << err_connect_file << '\n';
		std::cout << quest_create_file;
		bool isRw = false;
		char assent;
		
		do
		{
			std::cin >> assent;
			if (assent == 'n')
			{
				return 0;
			}
			if (assent == 'y')
			{
				isRw = true;
			}
			else
			{
				std::cout << quest_create_file;
				std::cin >> assent;
			}
		} while (!isRw);

		std::string host_in;
		std::string port_in;
		char dbname_in[size_connect_params]{};
		char dbuser_in[size_connect_params]{};
		char dbuser_in[size_connect_params]{};

		std::cout << enter_connect_params;
		std::cout << host_quest;
		std::getline(std::cin, host_in);
		std::cout << port_quest;
		std::getline(std::cin, port_in);
		std::cout << name_quest;
		std::cin >> dbname_in;
		std::cout << user_quest;
		std::cin >> dbuser_in;
		const char* pass_in = getpass(password_quest);

		strcat(temp, host_param);
		if (host_in.length() == 0)
		{
			strcat(temp, local_host);
			strcat(temp, " ");
		}
		else
		{
			const char* hostname = host_in.c_str();
			strcat(temp, hostname);
			strcat(temp, " ");
		}
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
