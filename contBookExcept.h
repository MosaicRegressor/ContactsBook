#ifndef CBOOK_EXCEPT
#define CBOOK_EXCEPT

#include <exception>
#include <string>

class BookSizeTooBig : public std::exception{   // thrown after bad alloc
    int _bad_size;
    public:
        BookSizeTooBig(int bad_size);
        const char * what();
};


class ContactAlreadyExistsInStorage : public std::exception{ // thrown after search in the book is done
    int _bad_tel;
    public:
        ContactAlreadyExistsInStorage(int bad_tel);
        const char * what();
};

class CBookStorageEmpty : public std::exception{ // thrown if trying to operate on empty storage book
    const char * _errorMsg;
    
    public:
        CBookStorageEmpty(const char * errorMsg);
        const char * what();
};

class CBookStorageFull : public std::exception{ // thrown if trying to add to a full storage book
    public:
        CBookStorageFull();
        const char * what();
};

class CBookDumpNotExists : public std::exception{ // thrown if trying to add to a full storage book
    public:
        CBookDumpNotExists();
        const char * what();
};

#endif