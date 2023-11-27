#ifndef MY_USER
#define MY_USER

#include <exception>
#include <string>
#include <memory>


namespace my
{
    class Exception : public std::exception
    {
        private:
        const char* msg;
    
        public:
            Exception(const char* msg_) : msg(msg_){}
            virtual ~Exception(){}
    
            const char* what() const noexcept override
            {
                return msg;
            }
    };

    class User
    {
        private:
            std::string name;
            std::unique_ptr<std::string> pass_hash;
        public:
            User();
            User(const std::string name, const std::string* pass_hash);
            virtual ~User();

            void save();
    };
};

#endif