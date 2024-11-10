#include  <iostream>
#include <string.h>
#include "contactsBook.h"
#include "contact.h"

int main(){
    std::cout << "Numero voci rubrica:" << std::endl;
    unsigned int maxContacts;
    std::cin >> maxContacts;

    ContactsBook contBook(maxContacts);

    std::string surname = "";
    std::string name = "";
    unsigned int nTel = 0;

    unsigned int i = 0;
    do{
        std::cout << "Inserisci cognome:" << std::endl;
        std::cin >> surname;
        std::cout << "Inserisci nome:" << std::endl;
        std::cin >> name;
        std::cout << "Inserisci nTel:" << std::endl;
        std::cin >> nTel;

        // add to rubrica
        contBook.push(surname, name, nTel); 
    }
    while(surname != "*" && i < maxContacts);

    std::cout << contBook << std::endl;

    return 0;
}