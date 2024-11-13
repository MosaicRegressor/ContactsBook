#include "ContactsBook.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>

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
                eraseStorageContent(); 
                _contactsInStorage = 0; // removed all of the contacts
            }
            delete[] _storage; _storage = nullptr;
            _storage = tmp;
            _maxContacts = newMaxContacts;
        }
    }
}

void ContactsBook::save(std::string fPath) const{
    assert(fPath != "");

    if(_contactsInStorage == 0){    // handling of base case
        std::cout << std::endl << "No saved contacts, nothing to dump..." << std::endl;
    }
    else{   // TODO handle errors
        std::ofstream fStream;
        try{
            fStream.open("contacts.dmp", std::ofstream::out | std::ofstream::trunc);
        }catch(...){
            std::cerr << "error in opening file, reverting...";
            }; // TODO delete file

        fStream << _contactsInStorage << "\n";
        for(unsigned int i = 0; i < _contactsInStorage; i++){
            Contact* contact =_storage[i];
            std::string surname = contact->_surname;
            std::string name = contact->_name;
            unsigned int tel = contact->_tel;
            try{
                fStream << surname << "\n" << name << "\n" << tel << "\n";
            }catch(...){
                std::cerr << "error in writing to file, reverting...";
            }; // TODO delete file
        }
        fStream.close();
    }
}

void ContactsBook::load(std::string fPath){     // precondition: the user has not tampered the backup file
    assert(fPath != "");
    // FIXME handle the case in which the file to be read doesn't exist

    // precondition: once opened, the read from a file will never fail 
    std::ifstream fStream;
    try{
        fStream.open("contacts.dmp", std::ofstream::in);
    }catch(...){
        std::cerr << "error in opening file, reverting...";
    }; // TODO delete file

    // get content of file 
    std::string line = "";
    std::getline(fStream, line);    //0
    
    // cast by using streams, the c++ way
    std::stringstream lineStream(line);
    unsigned int bkStorageSize = 0;
    lineStream >> bkStorageSize;

    Contact** restoredStorage = new Contact*[bkStorageSize];
    std::string surname = "";
    std::string name = "";
    unsigned int tel = 0;
    for(unsigned int i = 0; i < bkStorageSize; i++){
        std::getline(fStream, line);
        surname = line;
        std::getline(fStream, line);
        name = line;
        std::getline(fStream, line);
        std::stringstream telSStream(line);
        telSStream >> tel;
        restoredStorage[i] = new Contact{surname, name, tel};
    }
    fStream.close();

    this->_contactsInStorage = bkStorageSize;
    this->_maxContacts = bkStorageSize;

    eraseStorageContent();
    delete[] _storage;
    _storage = nullptr;

    _storage = restoredStorage;
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