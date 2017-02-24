#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

namespace depthmapX
{
    class BaseException : public std::exception
    {
    protected:
        BaseException(){}
        BaseException(std::string message) : _message(message)
        {}
    public:
        virtual const char * what() const noexcept
        {
            return _message.c_str();
        }
    private:
        std::string _message;
    };

    class CommandLineException : public BaseException
    {
    public:
        CommandLineException(std::string message) : BaseException(message)
        {}
    };

    class SetupCheckException : public BaseException
    {
    public:
        SetupCheckException(std::string message) : BaseException(message)
        {}
    };

    class RuntimeException: public BaseException
    {
    public:
        RuntimeException(std::string message) : BaseException(message)
        {}
    };

}

#endif // EXCEPTIONS_H
