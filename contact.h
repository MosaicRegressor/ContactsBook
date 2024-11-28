#ifndef CONTACT_H
#define CONTACT_H

#include <string.h>
#include <iostream>

struct Contact {
    std::string _surname;
    std::string _name;
    int _tel;
};

std::ostream &operator<<(std::ostream &os, const Contact &contact);

#endif