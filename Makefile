all: yees

yees: email.o register.o validation.o sign_In.o crypt.o head.o
	g++ email.o register.o validation.o sign_In.o crypt.o head.o -lsqlite3 -lcrypto++ -std=c++11 -o yees
	
email.o: email.cpp
	g++ -c -std=c++11 email.cpp

register.o: register.cpp
	g++ -c -std=c++11 register.cpp

validation.o: validation.cpp
	g++ -c -std=c++11 validation.cpp
	
sign_In.o: sign_In.cpp
	g++ -c -std=c++11 sign_In.cpp
	
crypt.o: crypt.cpp
	g++ -c -std=c++11 crypt.cpp
	
head.o: head.cpp
	g++ -c -std=c++11 head.cpp
	
clean:
	rm *.o yees