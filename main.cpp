#include <iostream>
#include <string.h>
#include "ContactsBook.h"
#include "Contact.h"

#define DEBUG

/*
TODO
- implement exception handling
- implement feat load and save
*/

int main(){

    unsigned int maxContacts;
    #ifndef DEBUG
    std::cout << "Numero voci rubrica:";
    std::cin >> maxContacts;
    #endif

    #ifdef DEBUG
    maxContacts = 5;
    #endif

    ContactsBook contBook(maxContacts);

    std::string surname = "";
    std::string name = "";
    unsigned int tel = 0;

    unsigned int i = 0;
    bool wantsToInput = true;
    while(wantsToInput && i < maxContacts){
        #ifdef DEBUG
        surname = "doe"; name = "john"; tel = i;
        #endif
        #ifndef DEBUG
        std::cout << std::endl << "Inserisci cognome:";
        std::cin >> surname;
        #endif

        #ifndef DEBUG
        std::cout << std::endl << "Inserisci nome:";
        std::cin >> name;
        #endif

        #ifndef DEBUG
        std::cout << std::endl << "Inserisci nTel:";
        std::cin >> tel;
        #endif

        wantsToInput = surname != "*";
        if(wantsToInput){
            contBook.push(surname, name, tel);
        }
        i++;    // FIXME even if the push is not done, this is incremented
    }

    #ifdef DEBUG
    std::cout << std::endl << "first init";
    std::cout << contBook;

    std::cout << std::endl << "Extend by one";
    contBook.set_max_contacts(maxContacts + 1);
    std::cout << contBook;

    std::cout << std::endl << "Dump file";
    contBook.save("contacts.dump");

    std::cout << std::endl << "Read dump file";
    contBook.load("contacts.dump");

    std::cout << std::endl << "After load of backup";
    std::cout << contBook;

    std::cout << std::endl << "Shrink by one";
    contBook.set_max_contacts(maxContacts - 1);
    std::cout << contBook;
    #endif


    return 0;
}