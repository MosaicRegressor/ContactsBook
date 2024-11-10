#include "contactsBook.h"
#include <cassert>
#include <iostream>
#include <fstream>

// deafult constructor
ContactsBook::ContactsBook() : _maxContacts(0), _storage(nullptr), _contactsInMemory(0){
    #ifndef NDEBUG
    std::cout << "ContactsBook()" << std::endl;
    #endif
};

ContactsBook::ContactsBook(unsigned int maxContacts) : _maxContacts(0), _storage(nullptr), _contactsInMemory(0) {
    _maxContacts = maxContacts;
    
    #ifndef NDEBUG
    std::cout << "ContactsBook(maxContacts)" << std::endl;
    #endif
};

// copy constructor
ContactsBook::ContactsBook(ContactsBook &other) : _maxContacts(0), _storage(nullptr), _contactsInMemory(0) {
                                                        
    Contact** tmp = new Contact*[other._maxContacts];  // come prima cosa cerco di allocare, assicuro la recovery dall'error
    
    for(unsigned int i = 0; i < other._maxContacts; i++){
        tmp[i] = other._storage[i];
    }

    _contactsInMemory = other._contactsInMemory;
    _maxContacts = other._maxContacts;
    _storage = tmp;
}

ContactsBook& ContactsBook::operator=(ContactsBook &other){
    if(this != &other){
        ContactsBook tmp(other);    // recovery errore

        std::swap(this -> _storage, tmp._storage);
        std::swap(this -> _maxContacts, tmp._maxContacts);
        std::swap(this -> _contactsInMemory, tmp._contactsInMemory);
    }
}

// FIXME
Contact& ContactsBook::operator[](unsigned int index){
    assert(index < _maxContacts);
    return _storage[index];
}

ContactsBook::~ContactsBook(){
    delete[] _storage;
    _storage = nullptr;
    _maxContacts = 0;
    _contactsInMemory = 0;
}

void ContactsBook::set_maxContacts(unsigned int newMaxContacts){
    // provo ad allocare un array di puntatori a struct voce
    Contact** tmp = new Contact*[newMaxContacts];

    // se ho voci in rubrica e voglio estenderla
    if(_contactsInMemory > 0 && (newMaxContacts > _maxContacts)){
        // mi segno di quanto la estendo
        unsigned int numNewSlots = newMaxContacts - _maxContacts;
        // copia vecchi elementi
        for(unsigned int i = 0; i < _contactsInMemory; i++){
            tmp[i] = _storage[i];
        }
        // initialization of remaining blank slots to defined behavior
        for(unsigned int i = _contactsInMemory; i < numNewSlots; i++){
            tmp[i] = nullptr;
        }
        delete[] _storage;
        _storage = tmp;
    }
    else {
        // reset content of contactsBook and set new instance state
        delete[] _storage;
        _storage = tmp;
        emptyMemory();
    }
    _maxContacts = newMaxContacts;
}

void ContactsBook::emptyMemory(){
    // azzero tutte le celle
    for(int i = 0; i < _maxContacts; i++){
        _storage[i] = nullptr;
    }
    _contactsInMemory = 0;
}

// FIXME
Contact& ContactsBook::search(unsigned int tel) const{
    unsigned int index = 0;
    while(index < _contactsInMemory && _storage[index]->_tel != tel){
        index++;
    }
    if(index == _contactsInMemory){   // not found, out of bounds
        Contact v1 = {0, 0, 0};
        return v1;
    }
    else {
        return _storage[index];
    }
}

void ContactsBook::push(Contact contact){
    if (_contactsInMemory < _maxContacts){  // if storage not full
        if (search(contact._tel)._tel != 0){     // if contact not already in memory
            Contact* tmp = new Contact{contact._surname, contact._name, contact._tel};
            _storage[_contactsInMemory] = tmp;
            _contactsInMemory++;
        }
        else{
             std::cout << "Contact already exists!" << std::endl;
        }
    }
    else{
        std::cout << "Contacts book is full!" << std::endl;
    }
}

void ContactsBook::push(std::string surname, std::string name, unsigned int tel){
    Contact contact = Contact{surname, name, tel};
    push(contact);
}

std::ostream &operator<<(std::ostream &os, const ContactsBook &contactsBook){
    os << "Size of the rubrica: " << contactsBook._maxContacts << std::endl;
    os << "Number of saved items: " << contactsBook._contactsInMemory;
    
    Contact* contact = nullptr;
    for(unsigned int i = 0; i < contactsBook._contactsInMemory; i++){
        contact = contactsBook._storage[i];
        os << "#" << i << " :" << *contact << std::endl;
    }
    return os;
}
