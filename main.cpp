#include <iostream>
#include <exception>
#include <fstream>
#include <pqxx/pqxx>


static const char* err_overload_size_file = "This application limit 256 chars in file.";
static const int data_chars_in_file = 256;

class MyException : public std::exception
{
	private:
		const char* msg;

	public:
		MyException(const char* msg_) : msg(msg_){}
		virtual ~MyException(){}

		const char* what() const noexcept override
		{
			return msg;
		}
};

const char* file_to_chars(const char* path, char devider)
{
	std::cout << path << " with devider: \"" << devider << "\"\n";
	std::ifstream fin(path);
	if (fin.fail())
	{
		throw MyException(err_open_file_msg);
	}
	
	char symvol;
	char data[data_chars_in_file];
	int i = 0;

	while (fin.get(symvol))
	{
		if (i >= data_chars_in_file)
		{
			throw MyException(err_overload_size_file);
		}
		if (symvol == '\n')
		{
			data[i] = devider;
		}
		else
		{
			strcat(data, symvol);
		}
	}

	return static_cast<const char*>(data);
} 

int main()
{
	const char* err_connect_file = "File not exist, or heve fail parametres.";
	const char* quest_create_file = "You do want create new file? (y/n): ";
	const char* path_connect = "connect_db.txt";
//	const std::string path_create_table = "../create_tb.sql";
//	std::string sql_str = "";
//	const std::string sql_str;
	const char* connect_str;
	bool isConnect = false;

	try
	{
		connect_str = file_to_chars(path_connect, " ");
		isConnect = true;
	}
	catch(std::exception e)
	{
		std::cout << e.what() << path_connect << '\n';
		std::cout << err_connect_file << '\n';
		std::cout << quest_create_file;		
	}

//		sql_str = file_to_string(path_create_table);
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
