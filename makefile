main.exe: main.o ContactsBook.o Contact.o
	g++ main.o ContactsBook.o Contact.o -o main

main.o:	main.cpp
	g++ -c main.cpp -p main.o

ContactsBook.o:	ContactsBook.cpp ContactsBook.h
	g++ -c ContactsBook.cpp -p ContactsBook.o

Contact.o:	Contact.cpp Contact.h
	g++ -c Contact.cpp -p Contact.o