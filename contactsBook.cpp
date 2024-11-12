#include "ContactsBook.h"
#include <cassert>
#include <iostream>
#include <fstream>

// default constructor
ContactsBook::ContactsBook() : _maxContacts(0), _storage(nullptr), _contactsInStorage(0){
    #ifdef DEBUG
    std::cout << std::endl << "ContactsBook()";
    #endif
};

ContactsBook::ContactsBook(unsigned int maxContacts) : _maxContacts(0), _storage(nullptr), _contactsInStorage(0) {
    #ifdef DEBUG
    std::cout << std::endl <<  "ContactsBook(maxContacts)";
    #endif
    set_max_contacts(maxContacts);   // with this setter istantiation of the storage
};

// copy constructor
ContactsBook::ContactsBook(ContactsBook &other) : _maxContacts(0), _storage(nullptr), _contactsInStorage(0) {
    #ifdef DEBUG
    std::cout << std::endl << "Copy constructor";
    #endif                                          
    Contact** tmp = new Contact*[other._maxContacts];  // come prima cosa cerco di allocare, assicuro la recovery dall'error
    
    for(unsigned int i = 0; i < other._maxContacts; i++){
        tmp[i] = other._storage[i];
    }

    _contactsInStorage = other._contactsInStorage;
    _maxContacts = other._maxContacts;
    delete[] _storage;
    _storage = tmp;
}

ContactsBook& ContactsBook::operator=(ContactsBook &other){
    if(this != &other){
        ContactsBook tmp(other);    // recovery errore

        std::swap(this -> _storage, tmp._storage);
        std::swap(this -> _maxContacts, tmp._maxContacts);
        std::swap(this -> _contactsInStorage, tmp._contactsInStorage);
    }
    return *this;
}

Contact& ContactsBook::operator[](unsigned int index){
    assert(index < _maxContacts);
    return *_storage[index];
}

ContactsBook::~ContactsBook(){
    #ifdef DEBUG
    std::cout << std::endl << "Destructor()";
    #endif

    eraseStorageContent();
    delete[] _storage;
    _storage = nullptr;
    _maxContacts = 0;
    _contactsInStorage = 0;
}


/*
3 kinds of input:
1) _storage doesn't exist
    - allocate new _storage of the desidered size, init all the cells to nullptr, assign it to _storage

2) _storage exists and contains contacts
    - extend: allocate new storage of the desidered size, fill in the existing contacts, init the remaining cells, delete old storage, swap
    - shrink: allocate new storage of the desired size, init all the cells to nullptr, delete old storage, assign new storage

3) _storage exists and contains nullptr in all cells
    - extend/shrink: allocate new storage of the new size, init all the cells to nullptr

steps
1) allocate the new storage and init its cells to defined behavior
2) is this the first start? create storage and assign it
3) if it is not first start, do you want to shrink or extend?
    if shrink, deep erase of old memory, assign new memory
    if extend, fill new memory, deep erase of old memory, assign new memory

*/
void ContactsBook::set_max_contacts(unsigned int newMaxContacts){
    #ifdef DEBUG
    std::cout << std::endl << "SetMaxContacts()";
    #endif

    // handling of base case
    if(newMaxContacts == 0){    // the user wants to delete the storage
        eraseStorageContent();
        delete[] _storage;
        _storage = nullptr;
        _maxContacts = 0;
        _contactsInStorage = 0;
    }
    else{
        // allocate and init new storage
        Contact** tmp = new Contact*[newMaxContacts];
        for(unsigned int i = 0; i < newMaxContacts; i++){   // FIXME lento!! ottimizza!
            tmp[i] = nullptr;
        }

        // understand in which case you are
        if(_storage == nullptr){    // first run, just assign the new storage
            _storage = tmp;
            _maxContacts = newMaxContacts;
            _contactsInStorage = 0;
        }
        else{   // there's already a storage
            if(newMaxContacts > _maxContacts){  // let's extend, fill in the contacts if they exist, then set the new memory
                // fill the contacts in the new storage, then swap storage
                for(unsigned int i = 0; i < _contactsInStorage; i++){    // if no contacts, no contacts will be inserted
                    tmp[i] = _storage[i];
                }
                // _contactsInStorage is unchanged, i keep the contacts

                // now the content of _Storage is SHARED with _tmp
            }
            else{   // shrink, delete content
                _contactsInStorage = 0; // removed all of the contacts
                eraseStorageContent(); 
            }
            delete[] _storage; _storage = nullptr;
            _storage = tmp;
            _maxContacts = newMaxContacts;
        }
    }
}

// deallocate class contacts and restore defined behavior
void ContactsBook::eraseStorageContent(){
    #ifdef DEBUG
    std::cout << std::endl << "Empty memory()";
    #endif

    // deep erase OF THE CONTENTS of internal storage, then set content to defined behavior
    for(int i = 0; i < _contactsInStorage; i++){    
        delete _storage[i];
        _storage[i] = nullptr;
    }
}

// FIXME unsafe return type
Contact* ContactsBook::search(unsigned int tel) const{
    #ifdef DEBUG
    std::cout << std::endl << "search()";
    #endif

    assert(_storage != nullptr);

    if(_contactsInStorage == 0){
        return nullptr;
    }

    unsigned int index = 0;
    while(index < _contactsInStorage && _storage[index]->_tel != tel){
        index++;
    }

    if(index < _contactsInStorage){   // not found, out of bounds
        return _storage[index];
    }
    else {
        return nullptr;
    }
}

void ContactsBook::push(Contact* contact){
    #ifdef DEBUG
    std::cout << std::endl << "push(Contact*)";
    #endif
    assert(contact != nullptr);
    if (_contactsInStorage < _maxContacts){  // if storage not full

        bool contactExists = search(contact->_tel) != nullptr;
        if (!contactExists){     // if contact not already in storage
            _storage[_contactsInStorage] = contact;
            _contactsInStorage++;
        }
        else{
             std::cout << "Contact already exists, not added!" << std::endl;
             // TODO raise exception
        }
    }
    else{
        std::cout << "Contacts book is full, not added!" << std::endl;
        // TODO raise exception
    }
}

void ContactsBook::push(Contact contact){   // passed by copy
    #ifdef DEBUG
    std::cout << std::endl << "push(Contact)";
    #endif
    Contact* c2 = new Contact{contact._surname, contact._name, contact._tel};
    push(c2);
}

void ContactsBook::push(std::string surname, std::string name, unsigned int tel){
    #ifdef DEBUG
    std::cout << std::endl << "push(params)";
    #endif
    Contact* contact = new Contact{surname, name, tel};
    push(contact);
}

std::ostream &operator<<(std::ostream &os, const ContactsBook &contactsBook){
    os << std::endl << "Rubrica capacity: " << contactsBook._maxContacts;
    os << std::endl << "Saved items: " << contactsBook._contactsInStorage;
    os << std::endl;

    for(unsigned int i = 0; i < contactsBook._contactsInStorage; i++){
        os << "Contact #" << i + 1 << " :" << *(contactsBook._storage[i]) << std::endl;
    }
    return os;
}