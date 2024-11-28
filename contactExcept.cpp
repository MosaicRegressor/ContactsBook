#include "contactExcept.h"

// the best choice would be to implement validation on contact fields at the instantiation of the contact, but by requirements we must define contact as a struct
// so we define separate function to validate a contact
ContactInfoNotValid::ContactInfoNotValid(std::string msg) : _errorMsg(msg){}
const char * ContactInfoNotValid::what() {
    return "Some contact fields are not valid!";
}