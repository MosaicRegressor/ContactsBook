#include <iostream>
#include <string.h>
#include "ContactsBook.h"
#include "Contact.h"

#define DEBUG

/*
TODO
- implement exception handling
- implement feat load and save
- test shrink with initial storage size = 1
*/

int main(){
    unsigned int maxContacts;
    std::cout << "Numero voci rubrica:";
    std::cin >> maxContacts;

    ContactsBook contBook(maxContacts);

    std::string surname = "";
    std::string name = "";
    unsigned int tel = 0;

    unsigned int i = 0;
    bool wantsToInput = true;
    while(wantsToInput && i < maxContacts){
        #ifdef DEBUG
        surname = i; name = i; tel = i;
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

    std::cout << contBook;

    // extend
    #ifdef DEBUG
    std::cout << std::endl << "Extend by one";
    #endif
    contBook.set_max_contacts(maxContacts + 1);
    std::cout << contBook;

    // shrink
    #ifdef DEBUG
    std::cout << std::endl << "Shrink by one";
    #endif
    contBook.set_max_contacts(maxContacts - 1);
    std::cout << contBook;


    return 0;
}