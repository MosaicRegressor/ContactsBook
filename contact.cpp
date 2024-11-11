#include <string.h>
#include <iostream>
#include "Contact.h"

std::ostream &operator<<(std::ostream &os, const Contact &contact){
    return os << contact._surname << " " << contact._name << " " << contact._tel << std::endl;
}