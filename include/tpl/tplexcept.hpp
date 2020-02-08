#ifndef TPL_EXCEPT_H_
#define TPL_EXCEPT_H_

#include <exception>
#include <string>

namespace tpl
{
    class task_canceled : public std::exception
    {
    private:
        std::string message_;

    public:
        explicit task_canceled(const char* msg) throw() : message_(msg) {}

        task_canceled() throw() : exception() {}

        ~task_canceled() throw() {}

        const char* what() const noexcept { return message_.c_str(); }
    };

    class invalid_operation : public std::exception
    {
    private:
        std::string message_;

    public:
        invalid_operation(const char* msg) throw() : message_(msg) {}

        invalid_operation() throw() : exception() {}

        ~invalid_operation() throw() {}

        const char* what() const noexcept { return message_.c_str(); }
    };
}

#endif // TPL_EXCEPT_H_