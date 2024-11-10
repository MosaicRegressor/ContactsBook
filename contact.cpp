#include <string.h>
#include <iostream>
#include "contact.h"

std::ostream &operator<<(std::ostream &os, const Contact &contact){
    return os << contact._surname << " " << contact._name << " " << contact._tel << std::endl;
}