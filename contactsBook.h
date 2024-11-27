#ifndef CONTACTS_BOOK
#define CONTACTS_BOOK

#include "Contact.h"
#include <iostream>

class ContactsBook{ // implemented as a stack
    unsigned int _maxContacts;
    Contact** _storage;   // array of pointers to contacts structs
    unsigned int _contactsInStorage; // used to keep track of the fullness of the stack

    private:
        void push(Contact* contact);

    public:
        // default constructor, FUNDAMENTAL
        ContactsBook();

        // optional constructor, OPTIONAL
        // instantiate storage of the book with user defined size and init class variables
        ContactsBook(int maxContacts);     // uses setMaxSize()

        // copy constructor, FUNDAMENTAL
        // instatiate copy of another object
        ContactsBook(ContactsBook &other);

        // destructor, FUNDAMENTAL
        // deallocate storage, set defined behavior
        ~ContactsBook();    // uses eraseStorage()

        // override of operator =, FUNDAMENTAL
        ContactsBook& operator=(ContactsBook &other);

        // override operator [] for syntactic sugar
        Contact& operator[](unsigned int index);

        // setter
        // sets new storage size for an existing book.
        // If the size is greater than the current one, keep contacts and extend the storage.
        // If the size is lower than the current one, deep erase current contacts, set storage cells to defined behavior and shrink size.
        // If the size specified by the user is 0, the storage is deep erased and defined behavior is restored
        void set_max_contacts(int maxContacts);  // uses eraseStorageContent()      

        // zero the contents of memory
        void eraseStorageContent();

        // fill the book storage with the content of the backup file
        // backup file: 1st line number of records, then they are all records
        //              record structure: 
        void load();    // uses eraseStorageContent()

        void save() const;

        // search in book by telephone.
        // if the contact is found in the storage return its pointer, if not return nullptr
        Contact* search(unsigned int tel) const;

        // try to push a contact in the storage of the book
        // interfaces for user
        void push(Contact contact);
        void push(std::string surname, std::string name, unsigned int tel);

        friend std::ostream &operator<<(std::ostream &os, const ContactsBook &contactsBook);
};

// override << operator for syntactic sugar
std::ostream &operator<<(std::ostream &os, const ContactsBook &contactsBook);

#endif