#include <iostream>
#include <string.h>
#include "contactsBook.h"
#include "contact.h"
#include "contBookExcept.h"
#include "contactExcept.h"

// #define DEBUG


// TODO rewrite the debug blocks
int main(){
    int maxContacts = 0;
    bool validSize = false;
    do{
        #ifndef DEBUG
        std::cout << "Numero voci rubrica:";
        std::cin >> maxContacts;
        #endif
        #ifdef DEBUG
        maxContacts = 5;
        isInputValid = true;
        #endif
        try {
            ContactsBook tmp(maxContacts); // it only exists in this block beacuse of RAII, if there's problem in init, it cannot exist
            validSize = true;
        }
        catch(std::range_error &e){
            std::cout << e.what();   
        }
    }
    while(!validSize);

    ContactsBook contBook(maxContacts);

    std::string surname = ""; std::string name = ""; int tel = 0;
    int i = 0; bool wantsToInput = true;
    while(wantsToInput && i < maxContacts){
        #ifdef DEBUG
        surname = "doe"; name = "john"; tel = i;
        #endif
        #ifndef DEBUG
        std::cout << std::endl << "Insert surname:";
        std::cin >> surname;
        #endif
        #ifndef DEBUG
        std::cout << std::endl << "Insert name:";
        std::cin >> name;
        #endif
        #ifndef DEBUG
        std::cout << std::endl << "Insert phone number:";
        std::cin >> tel;
        #endif
        wantsToInput = surname != "*";
        if(wantsToInput){
            try{
                contBook.push(surname, name, tel);
                i++;
            }
            catch(ContactAlreadyExistsInStorage &e){
                std::cout << e.what();
            }
            catch(CBookStorageFull &e){
                std::cout << e.what();
            }
            catch(ContactInfoNotValid &e){
                std::cout << e.what();
            }
            catch(CBookStorageEmpty &e){
                std::cout << e.what();
            }
        }
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
    #ifndef DEBUG
    std::cout << contBook;

    std::string wantsToSave = "";
    std::cout << std::endl << "Do you want to save a backup of the contacts book?(y/n):";
    std::cin >> wantsToSave;
    if(wantsToSave == "y"){
        try{
            contBook.save();
            contBook.load();
            std::cout << contBook;
        }
        catch(CBookStorageEmpty &e){
            std::cout << e.what();
        }
        catch(CBookDumpNotExists &e){
            std::cout << e.what();
        }
    }
    #endif

    return 0;
}