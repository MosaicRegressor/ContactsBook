#include "contactsBook.h"
#include "contBookExcept.h"
#include "contactExcept.h"
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
    
    for(int i = 0; i < other._maxContacts; i++){
        tmp[i] = other._storage[i];
    }

    _contactsInStorage = other._contactsInStorage;
    _maxContacts = other._maxContacts;
    delete[] _storage;
    _storage = tmp;
}

// getters, returned read only reference
const int& ContactsBook::maxContacts() const{
    return _maxContacts;
}
const int& ContactsBook::contactsInStorage() const{
    return _contactsInStorage;
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

Contact& ContactsBook::operator[](int index){
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
    else if(newMaxContacts == _maxContacts){
        std::cout << "The new size is the same as the current size, nothing to do";
    }
    else if(newMaxContacts == 0){   // the user wants to delete the storage
        eraseStorageContent();
        delete[] _storage;
        _storage = nullptr;
        _maxContacts = 0;
        _contactsInStorage = 0;
    }
    else{   // the storage has contacts saved
        Contact** tmp = nullptr;
        try{
            tmp = new Contact*[newMaxContacts];
        }
        catch(std::bad_alloc){
            throw std::range_error("The new size specified for the storage is too big or there's no more ram available!");
        }

        for(int i = 0; i < newMaxContacts; i++){
            tmp[i] = nullptr;
        }

        if(_storage == nullptr){    // call at init
            _storage = tmp;
            _maxContacts = newMaxContacts;
            _contactsInStorage = 0;
        }
        else{
            if(newMaxContacts > _maxContacts){  // expand, keep data, just add cells
                for(int i = 0; i < _contactsInStorage; i++){
                    tmp[i] = _storage[i];
                }
            }
            else{   // shrink, deep erase data, remove cells
                eraseStorageContent();
                _contactsInStorage = 0;
            }
            // swap storage with the new one
            delete[] _storage; _storage = nullptr;
            _storage = tmp;
            _maxContacts = newMaxContacts;
        }
    }
}

// precondition: once opened, the write from a file will never fail
void ContactsBook::save() const{
    if(_contactsInStorage == 0){
        throw CBookStorageEmpty("You can't save an empty storage!");
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
        for(int i = 0; i < _contactsInStorage; i++){
            Contact* contact =_storage[i];
            std::string surname = contact->_surname;
            std::string name = contact->_name;
            int tel = contact->_tel;
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
    
    int bkStorageSize = 0; std::stringstream lineStream(line); lineStream >> bkStorageSize;

    Contact** restoredStorage = new Contact*[bkStorageSize];
    std::string surname = "";
    std::string name = "";
    int tel = 0;
    for(int i = 0; i < bkStorageSize; i++){
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

Contact* ContactsBook::search(int tel) const{
    #ifdef DEBUG
    std::cout << std::endl << "search()";
    #endif

    if(_maxContacts == 0){
        throw CBookStorageEmpty("You can't find contacts in an empty book!");
    }
    else if(_contactsInStorage == 0){
        return nullptr;
    }

    int index = 0;
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

void validateContactInfo(std::string name, std::string surname, int tel){
    std::string errorString = "";
    if(name == ""){
        errorString += "invalid name";
    }
    if(surname == ""){
        errorString += "invalid surname";
    }
    if(tel < 0){
        errorString += "invalid tel number";
    }
    if(errorString != ""){
        throw ContactInfoNotValid(errorString);
    }
}

void ContactsBook::push(Contact* contact){
    #ifdef DEBUG
    std::cout << std::endl << "push(Contact*)";
    #endif
    if (_contactsInStorage < _maxContacts){  // if storage not full
        bool contactExists = search(contact->_tel) != nullptr;
        if (!contactExists){     // if contact not already in storage
            _storage[_contactsInStorage] = contact;
            _contactsInStorage++;
        }
        else{
             std::cout << "Contact already exists, not added!" << std::endl;
             throw ContactAlreadyExistsInStorage(contact->_tel);
        }
    }
    else{
        throw CBookStorageFull();
    }
}

void ContactsBook::push(Contact contact){
    #ifdef DEBUG
    std::cout << std::endl << "push(Contact)";
    #endif

    std::string cName = contact._name;
    std::string cSurname = contact._surname;
    int cTel = contact._tel;
    
    try{
        validateContactInfo(cName, cSurname, cTel);
        Contact* c2 = new Contact{cSurname, cName, cTel};
        push(c2);
    }
    catch(std::bad_alloc){
        std::cout << "Unable to allocate copy of the contact, push not done";
    }
}

void ContactsBook::push(std::string surname, std::string name, int tel){
    #ifdef DEBUG
    std::cout << std::endl << "push(params)";
    #endif

    try{
        validateContactInfo(name, surname, tel);
        Contact* contact = new Contact{surname, name, tel};
        push(contact);
    }
    catch(std::bad_alloc){
        std::cout << "Unable to allocate copy of the contact, push not done";
    }
}

std::ostream &operator<<(std::ostream &os, const ContactsBook &contactsBook){
    os << std::endl << "Rubrica capacity: " << contactsBook._maxContacts;
    os << std::endl << "Saved items: " << contactsBook._contactsInStorage;
    os << std::endl;

    for(int i = 0; i < contactsBook._contactsInStorage; i++){
        os << "Contact #" << i + 1 << " :" << *(contactsBook._storage[i]) << std::endl;
    }
    return os;
}