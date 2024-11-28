#include "contBookExcept.h"

BookSizeTooBig::BookSizeTooBig(int bad_size) : _bad_size(bad_size) {}
const char * BookSizeTooBig::what() {
    return "Book size too big! size specified: " + _bad_size;
}

ContactAlreadyExistsInStorage::ContactAlreadyExistsInStorage(int bad_tel) : _bad_tel(bad_tel){}
const char * ContactAlreadyExistsInStorage::what() {
    return "Contact already exists! tel of the contact: " + _bad_tel;
}

CBookStorageEmpty::CBookStorageEmpty(const char * errorMsg) : _errorMsg(errorMsg){}
const char * CBookStorageEmpty::what() {
        return _errorMsg;
}

// TODO implement errorMsg, more descriptive
CBookStorageFull::CBookStorageFull(){}
const char * CBookStorageFull::what(){
    return "You are trying to add a contact to a book that has a full storage!";
}

CBookDumpNotExists::CBookDumpNotExists(){}
const char * CBookDumpNotExists::what(){
    return "The dump file doesn't exist, there's nothing to load!";
}