#ifndef MY_USER
#define MY_USER

#include <string>
#include <memory>


namespace my
{
    class User
    {
        private:
            std::string name;
            std::unique_ptr<std::string> pass_hash;
        public:

    };
};

#endif