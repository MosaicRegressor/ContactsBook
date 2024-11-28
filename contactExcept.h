#ifndef CONT_EXCEPT
#define CONT_EXCEPT

#include <exception>
#include <string>

class ContactInfoNotValid : public std::exception{ // thrown if one or more of the fields of the contacts is invalid
    std::string _errorMsg;
    public:
        ContactInfoNotValid(std::string msg);
        const char * what();
};

#endif