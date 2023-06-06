#include <unistd.h>
#include <iostream>
#include <exception>
#include <fstream>
#include <pqxx/pqxx>
#include "lib/my_file/my_file.h"


std::string& variant_string(std::string& orig_str, std::string& def_str)
{
	if (def_str.length() == 0)
	{
		return def_str;
	}
	else
	{
		return orig_str;
	}
}

std::string map_to_stp(
	std::map<std::string, std::string>& dict,
	const std::string& div_row,
	const std::string& div_col
	)
{
	std::string str = "";
	for (auto elem : dict)
	{
		str += elem.first + div_col + elem.second + div_row;
	}

	return str;
}

void dialog(
	std::vector<std::string>& quest, 
	std::map<std::string, std::string>& response
	)
{
	unsigned int i = 0;
	for ( const auto elem : response)
	{
		if (response[elem.first] == "password")
		{
			const char* pass_quest = quest[i].c_str();
			response[elem.first] = getpass(pass_quest);
		}
		std::cout << quest[i];
		std::getline(std::cin, response[elem.first]);
	}
}

int main()
{
	const char* path_connect = "connect_db.txt";
	const char* err_connect_file = "File not exist, or heve fail parametres.";
	const char* quest_create_file = "You do want create new file? (y/n): ";
	const char* enter_connect_params = "Enter params for connect DB:\n";

	const std::string str_divider = " ";
	const std::string file_divider = "\n";
	const std::string quest_divider = ": ";
	const std::string param_divider = "=";
	const std::string local_host = "localhost";
	const std::string default_port = "5432";
	const std::string host_param = "host";
	const std::string host_quest = host_param + "(default: " + local_host + "): ";
	const std::string port_param = "port";
	const std::string port_quest = port_param + "(default: " + default_port + "): ";
	const std::string name_quest = "dbname";
	const std::string user_quest = "user";
	const std::string password_quest = "password";
	std::vector<std::string> connect_lable{
		host_quest,
		port_quest,
		name_quest,
		user_quest,
		password_quest,
	};

	std::string concat_str = host_param;
//	const std::string path_create_table = "../create_tb.sql";
//	std::string sql_str = "";
//	const std::string sql_str;
	const char* connect_str;

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

		std::map<std::string, std::string> connect_param{
			{host_param, ""},
			{port_param, ""},
			{name_quest, ""},
			{user_quest, ""},
			{password_quest, ""},
		};

		std::cout << enter_connect_params;
		dialog(connect_lable, connect_param);
		std::string str = map_to_stp(connect_param, file_divider, param_divider);

		try
		{
			MyFile file_out(path_connect, str);
		}
		catch (std::exception e)
		{
			std::cout << e.what() << '\n';
		}

		connect_str = str.c_str();
	}

	try
	{
		pqxx::connection c(connect_str);

		pqxx::work tx{ c };

		const char* dbname = tx.query_value<const char*>("SELECT current_database()");
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
