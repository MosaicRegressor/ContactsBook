#ifndef CONTACTS_BOOK
#define CONTACS_BOOK

#include "Contact.h"
#include <iostream>

class ContactsBook{  // implemented as a stack
    unsigned int _maxContacts;
    Contact** _storage;   // array of pointers to contacts structs
    unsigned int _contactsInMemory; // used to save the fullness of the stack

    private:
        void push(Contact* contact);

    public:
        // default constructor, FUNDAMENTAL
        ContactsBook();

        // optional constructor, OPTIONAL
        ContactsBook(unsigned int maxContacts);

        // copy constructor, FUNDAMENTAL
        ContactsBook(ContactsBook &other);

        // destructor, FUNDAMENTAL
        ~ContactsBook();

        // override of operator =, FUNDAMENTAL
        ContactsBook& operator=(ContactsBook &other);

        // override operator [] for syntactic sugar
        Contact& operator[](unsigned int index);

        // getter e setter
        void set_maxContacts(unsigned int maxContacts);        

        // zero the contents of memory
        void emptyMemory();

        // search in book by telephone and return reference
        Contact* search(unsigned int tel) const;

        // push contact on top of ContactsBook stack
        void push(Contact contact);
        void push(std::string surname, std::string name, unsigned int tel);

        friend std::ostream &operator<<(std::ostream &os, const ContactsBook &contactsBook);
};

// override << operator for syntactic sugar
std::ostream &operator<<(std::ostream &os, const ContactsBook &contactsBook);

#endif