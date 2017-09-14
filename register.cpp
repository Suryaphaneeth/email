#include <iostream>
#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include "head.h"
#include "register.h"
#include "sqlite3pp/sqlite3pp.h"
#include "crypt.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <list>
#include <algorithm>

using namespace std;
using namespace sqlite3pp;


int regis(){
    
    string userName;
    string passWord;
    string hashUserName;
    string hashPassword;
    bool check = true;

    database db("user_info.db");
    db.execute("create table USERS (UserName text , PassWord text, PRIMARY KEY(UserName))");
    registerHead();

    while(check){
        
        cout << endl << "Username: ";
        cin >> userName;
        
        //Query to check whether the username is already taken or not
        query qry(db, "SELECT UserName from USERS where UserName = :username");
        qry.bind(":username", userName, nocopy);
        if (qry.begin() != qry.end()) {
            cout << "(!_!)" << endl;
            cout << "The username: '" << userName<< "' already exists" << endl;
            cout << "Let's try something different (^_^)" << endl;
            check = true;
        }
        else
            check = false;
        
    }
    
    //If the username doesn't exist, then go for the password.
    while(!check){

        passWord = getpass("Password: ");
        if(passWord.size() < 6){
            cout << "Password is too short. Please enter a new password." << endl;
            check = false;
        }
        else if(passWord.compare(userName) == 0){
            cout << "Username and password can't be same. Please enter a new password." << endl;
            check = false;
        }
        else
            check = true;
    }
    
    cout << "You have successfully created your login details." << endl;
    // cout << "Your Username: " << userName << endl;
    // cout << "Your Password: " << passWord << endl;
    
    hashPassword = getHash(passWord);
    
    //Store the values of the username and the pasword 
    command cmd(db, "INSERT INTO USERS (UserName, PassWord) VALUES (?,?)");
    cmd.binder() << userName << hashPassword ;
    cmd.execute();
    
    cout << "Redirecting to main page..." << endl; 
     cin.ignore();
     cin.clear();
     cin.sync();
     cin.ignore(numeric_limits<streamsize>::max(),'\n');

    return 0;
}