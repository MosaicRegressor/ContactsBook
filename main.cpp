#include <iostream>
#include <string.h>
#include "ContactsBook.h"
#include "Contact.h"

/*
TODO
- implement exception handling
- implement feat load and save
- fix memory leak
*/

int main(){
    unsigned int maxContacts;
    std::cout << "Numero voci rubrica:";
    std::cin >> maxContacts;
    std::cout << std::endl;

    ContactsBook contBook(maxContacts);

    std::string surname = "";
    std::string name = "";
    unsigned int nTel = 0;

    unsigned int i = 0;
    bool wantsToInput = true;
    while(wantsToInput && i < maxContacts){
        std::cout << "Inserisci cognome:";
        std::cin >> surname;
        std::cout << std::endl;
        std::cout << "Inserisci nome:";
        std::cin >> name;
        std::cout << std::endl;
        std::cout << "Inserisci nTel:";
        std::cin >> nTel;
        std::cout << std::endl;

        wantsToInput = surname != "*";
        if(wantsToInput){
            contBook.push(surname, name, nTel);
        }
        i++;
    }

    std::cout << contBook << std::endl;

    return 0;
}