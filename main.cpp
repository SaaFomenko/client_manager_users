#include <unistd.h>
#include <iostream>
#include <exception>
#include <fstream>
#include <pqxx/pqxx>
#include "lib/my_file/my_file.h"


enum 
{
		lable = 0,
		key,
		value,
};

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

std::string table_to_str(
	std::vector<std::vector<std::string>>& table,
	const std::string& div_row,
	const std::string& div_col
	)
{
	std::string str = "";
	for (std::vector<std::string>& elem : table)
	{
		str += elem[key] + div_col + elem[value] + div_row;
	}

	return str;
}

void dialog(
	std::vector<std::vector<std::string>>& quest
	)
{
	if(char(std::cin.peek()) == '\n')
    	std::cin.ignore();

	if (std::cin.fail()) 
	{
    	std::cin.clear();
    	std::cin.ignore(32767, '\n');
	}

	for ( std::vector<std::string>& elem : quest)
	{
		if (elem[key] == "password")
		{
			const char* pass_quest = elem[lable].c_str();
			elem[value] = getpass(pass_quest);
		}
		else
		{
			std::string temp = elem[value];
			std::cout << elem[lable];
			std::getline(std::cin, elem[value]);

			if (elem[value].length() == 0)
			{
				elem[value] = temp;
			}
		}
	}
}

int main()
{
	const char* path_connect = "connect_db.txt";
	const char* err_connect_file = "File not exist, or heve fail parametres.";
	const char* quest_create_file = "You do want create new file? (y/n): ";
	const char* enter_connect_params = "Enter params for connect DB:\n";
	const char* greet_msg = "Wellcom, you connected to database: ";

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
	const std::string name_quest = "dbname: ";
	const std::string user_quest = "user: ";
	const std::string password_quest = "password: ";
	std::vector<std::vector<std::string>> connect_quests{
		{host_quest, host_param, local_host},
		{port_quest, port_param, default_port},
		{name_quest, "dbname", ""},
		{user_quest, "user", ""},
		{password_quest, "password", ""},
	};

//	const std::string path_create_table = "../create_tb.sql";
//	std::string sql_str = "";
//	const std::string sql_str;
	std::string str;

	try
	{
		MyFile file_read(path_connect);
		str = file_read.to_str();
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

		std::cout << enter_connect_params;
		dialog(connect_quests);
		str = table_to_str(connect_quests, file_divider, param_divider);

		try
		{
			MyFile file_out(path_connect, str);
		}
		catch (std::exception e)
		{
			std::cout << e.what() << '\n';
		}

		str = table_to_str(connect_quests, str_divider, param_divider);
	}

	try
	{
		const char* connect_str = str.c_str();
		pqxx::connection c(connect_str);

		pqxx::work tx{ c };

		const char* dbname = tx.query_value<const char*>("SELECT current_database()");
		std::cout << greet_msg << dbname << "\n";


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
