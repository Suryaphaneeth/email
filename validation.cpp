#include <iostream>
#include <string.h>
#include <vector>
#include "sqlite3pp/sqlite3pp.h"
#include "crypt.h"
#include <unistd.h>
#include "head.h"

using namespace std;
using namespace sqlite3pp;

vector<string> validation(vector<string> &vec){
    
    string userName;
    string passWord;
    string hashUserName;
    string hashPassword;
    bool choice = true;
    char prompt;
    
    database db("user_info.db");

    while(choice){
        signInhead();
        cout << "Username: ";
        cin >> userName;
        passWord = getpass("Password: ");
        
        //hashUserName = getHash(userName);
        hashPassword = getHash(passWord);
        
        query qry(db, "SELECT UserName, PassWord from USERS where UserName = :username and PassWord = :password");
        qry.bind(":username", userName, nocopy);
        qry.bind(":password", hashPassword, nocopy);
       
        if (qry.begin() != qry.end()) {
            cout << "Login Successful" << endl;
            choice = false;
            vec.at(0) = "True";
            vec.at(1) = userName;
            vec.at(2) = passWord;
            return vec;
        }
        else{
            cout << "Invalid credentials." << endl;
            cout << "Exit [E]       Try Again[T]" << endl;
            cout << "Your choice: ";
            cin >> prompt;
            while( !(toupper(prompt) == 'E' || toupper(prompt) == 'T') ){
                cout << endl;
                cout << "Please chose from following options: " << endl;
                cout << "Exit [E]       Try Again[T]" << endl;
                cout << "Your choice: ";
                cin >> prompt;
            }
            if( toupper(prompt) == 'E' ){
                choice = false;
                cout << "Returning to main menu..." << endl;
            }
        }
    }
    vec.at(0) = "False";
    vec.at(1) = "";
    vec.at(2) = "";
    return vec;
}