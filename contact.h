#include <string.h>
#include <iostream>

#ifndef CONTACT_H
#define CONTACT_H

struct Contact {
    std::string _surname;
    std::string _name;
    unsigned int _tel;
};

std::ostream &operator<<(std::ostream &os, const Contact &contact);

#endif