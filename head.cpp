#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>
#include <cstdlib>
// #include <conio.h>


using namespace std;

void clearScreen()
{
	try{
       system("clear");
	}
	catch (exception& e)
	{
        cout << e.what() << endl;
        cout << "The game is not running on unix like machine!" << endl;
        	try{
		       system("clear");
			}
			catch (exception& e)
			{
		        cout << e.what() << endl;
		        cout << "The game is not running on windows like machine! Aborting screen cleaning!" << endl;
			}
	}
}
void  chkanything()
{
    char chk; 
    int j;
    chk=getchar();
    j=chk;
    for(int i=1;i<=256;i++)
      if(i==j) break;
    clearScreen();
}
void signInhead(){
    clearScreen();
    cout << "|----------------------------------------------------------------------------------------------------------------|" << endl;
    cout << "| Sign-in to Yee-mail (Your encrypted email)                                                                     |" << endl;
    cout << "|----------------------------------------------------------------------------------------------------------------|" << endl;
    
}

void mainHead(){
    clearScreen();
    cout << "|----------------------------------------------------------------------------------------------------------------|" << endl;
    cout << "| Welcome to Yee-mail (Your encrypted email)                                                                     |" << endl;
    cout << "|----------------------------------------------------------------------------------------------------------------|" << endl;
    
}


void loginHead(string user){
    
    clearScreen();
    cout << "|----------------------------------------------------------------------------------------------------------------|" << endl;
    cout << "|Logged into YEES as " << user <<"                                                                                       |" << endl;
    cout << "|----------------------------------------------------------------------------------------------------------------|" << endl;
}

void inboxHead(string user){
    clearScreen();
    cout << "|----------------------------------------------------------------------------------------------------------------|" << endl;
    cout << "|Welcome to your Inbox " << user <<"                                                                                     |" << endl;
    cout << "|----------------------------------------------------------------------------------------------------------------|" << endl;
    
}

void composeHead(){
    
    clearScreen();
    cout << "|----------------------------------------------------------------------------------------------------------------|" << endl;
    cout << "|Compose an email                                                                                                |" << endl;
    cout << "|----------------------------------------------------------------------------------------------------------------|" << endl;
}

void settingsHead(){
    
    clearScreen();
    cout << "|----------------------------------------------------------------------------------------------------------------|" << endl;
    cout << "|Welcome to YEES settings                                                                                        |" << endl;
    cout << "|----------------------------------------------------------------------------------------------------------------|" << endl;
}

void registerHead(){
    clearScreen();
    cout << "|----------------------------------------------------------------------------------------------------------------|" << endl;
    cout << "|Welcome to Registration phase of YEES                                                                           |" << endl;
    cout << "|----------------------------------------------------------------------------------------------------------------|" << endl;
}


string getTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

void allMessages(){
    
    clearScreen();
    cout << "|----------------------------------------------------------------------------------------------------------------|" << endl;
    cout << "|All Messages                                                                                                    |" << endl;
    cout << "|----------------------------------------------------------------------------------------------------------------|" << endl;
}

void unReadMessages(){
    clearScreen();    

    cout << "|----------------------------------------------------------------------------------------------------------------|" << endl;
    cout << "|Unread Messages                                                                                                 |" << endl;
    cout << "|----------------------------------------------------------------------------------------------------------------|" << endl;
}

void deleteMessages(){
    clearScreen();    
    cout << "|----------------------------------------------------------------------------------------------------------------|" << endl;
    cout << "|Delete Messages                                                                                                 |" << endl;
    cout << "|----------------------------------------------------------------------------------------------------------------|" << endl;
}