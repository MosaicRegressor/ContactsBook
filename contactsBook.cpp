#include "ContactsBook.h"
#include "contBookExcept.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>

ContactsBook::ContactsBook() : _maxContacts(0), _storage(nullptr), _contactsInStorage(0){
    #ifdef DEBUG
    std::cout << std::endl << "ContactsBook()";
    #endif
};

ContactsBook::ContactsBook(int maxContacts) : _maxContacts(0), _storage(nullptr), _contactsInStorage(0) {
    #ifdef DEBUG
    std::cout << std::endl <<  "ContactsBook(maxContacts)";
    #endif
    set_max_contacts(maxContacts);
};

ContactsBook::ContactsBook(ContactsBook &other) : _maxContacts(0), _storage(nullptr), _contactsInStorage(0) {
    #ifdef DEBUG
    std::cout << std::endl << "Copy constructor";
    #endif                                          

    Contact** tmp = new Contact*[other._maxContacts];
    
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
    if(index < 0 || index >= _maxContacts){
        throw(std::out_of_range("index cannot be negative or greater than the size of the storage"));
    }
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

void ContactsBook::set_max_contacts(int newMaxContacts){
    #ifdef DEBUG
    std::cout << std::endl << "SetMaxContacts()";
    #endif

    if(newMaxContacts < 0){
        throw std::range_error("The storage of the book can't be negative!");
    }
    else if(newMaxContacts == 0){
        eraseStorageContent();
        delete[] _storage;
        _storage = nullptr;
        _maxContacts = 0;
        _contactsInStorage = 0;
    }
    else{
        Contact** tmp = nullptr;
        try{
            tmp = new Contact*[newMaxContacts];
        }
        catch(std::bad_alloc){
            throw std::range_error("The size specified is too big or there's no more ram available!");
        }

        for(unsigned int i = 0; i < newMaxContacts; i++){
            tmp[i] = nullptr;
        }

        if(_storage == nullptr){
            _storage = tmp;
            _maxContacts = newMaxContacts;
            _contactsInStorage = 0;
        }
        else{
            if(newMaxContacts > _maxContacts){
                for(unsigned int i = 0; i < _contactsInStorage; i++){
                    tmp[i] = _storage[i];
                }
            }
            else{
                eraseStorageContent(); 
                _contactsInStorage = 0;
            }
            delete[] _storage; _storage = nullptr;
            _storage = tmp;
            _maxContacts = newMaxContacts;
        }
    }
}

void ContactsBook::save() const{
    // precondition: once opened, the write from a file will never fail
    if(_contactsInStorage == 0){
        throw CBookStorageEmpty();
    }
    else{
        std::ofstream fStream;
        fStream.exceptions(std::ofstream::badbit | std::ofstream::failbit);
        try{
            fStream.open("contacts.dmp", std::ofstream::out | std::ofstream::trunc);    // excepted if file already opened
        }catch(std::ofstream::failure &e){
            std::cerr << "error in creating/opening file: " << e.what();
            return;
        };

        fStream << _contactsInStorage << "\n";
        for(unsigned int i = 0; i < _contactsInStorage; i++){
            Contact* contact =_storage[i];
            std::string surname = contact->_surname;
            std::string name = contact->_name;
            unsigned int tel = contact->_tel;
            fStream << surname << "\n" << name << "\n" << tel << "\n";
        }
        fStream.close();
        if(!fStream){
            std::remove("contacts.dmp");
            std::cerr << "an error happened while writing to file" << std::endl;
        }
    }
}

void ContactsBook::load(){     // precondition: the user has not tampered the backup
    // precondition: once opened, the read from a file will never fail
    std::ifstream fStream;
    fStream.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    try{
        fStream.open("contacts.dmp", std::ofstream::in);
        if(!fStream.is_open()){
            throw CBookDumpNotExists();
        }
    }catch(std::ofstream::failure &e){
        std::cerr << "error in opening file: " << e.what();
    };

    std::string line = "";std::getline(fStream, line);
    
    unsigned int bkStorageSize = 0; std::stringstream lineStream(line); lineStream >> bkStorageSize;

    Contact** restoredStorage = new Contact*[bkStorageSize];
    std::string surname = "";
    std::string name = "";
    unsigned int tel = 0;
    for(unsigned int i = 0; i < bkStorageSize; i++){
        std::getline(fStream, line); surname = line;
        std::getline(fStream, line); name = line;
        std::getline(fStream, line); std::stringstream telSStream(line); telSStream >> tel;
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

void ContactsBook::eraseStorageContent(){
    #ifdef DEBUG
    std::cout << std::endl << "Empty memory()";
    #endif

    for(int i = 0; i < _contactsInStorage; i++){    
        delete _storage[i];
        _storage[i] = nullptr;
    }
}

Contact* ContactsBook::search(unsigned int tel) const{
    #ifdef DEBUG
    std::cout << std::endl << "search()";
    #endif

    if(_contactsInStorage == 0){
        throw CBookStorageEmpty();
    }

    unsigned int index = 0;
    while(index < _contactsInStorage && _storage[index]->_tel != tel){
        index++;
    }

    if(index < _contactsInStorage){
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
    if(contact->_tel < 0){
        throw std::
    }
    if (_contactsInStorage < _maxContacts){  // if storage not full

        bool contactExists = search(contact->_tel) != nullptr;
        if (!contactExists){     // if contact not already in storage
            _storage[_contactsInStorage] = contact;
            _contactsInStorage++;
        }
        else{
             std::cout << "Contact already exists, not added!" << std::endl;
             throw ContactAlreadyExists(contact->_tel);
        }
    }
    else{
        std::cout << "Contacts book is full, not added!" << std::endl;
        throw CBookStorageFull();
    }
}

void ContactsBook::push(Contact contact){
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