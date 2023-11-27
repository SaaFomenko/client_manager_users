#include <iostream>
#include "../my_file/my_file.h"
#include "my_user.h"


static const char* my_profile = "profile.txt";

my::User::User()
{
    std::string str;

	MyFile file_read(my_profile);
	str = file_read.to_str();
}