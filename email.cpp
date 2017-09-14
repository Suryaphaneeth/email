#include <iostream>
#include <string.h>
#include <ctype.h>
#include <vector>
#include <algorithm>
#include <sstream>
#include "crypt.h"
#include "register.h"
#include "validation.h"
#include "sign_In.h"
#include "head.h"
#include "sqlite3pp/sqlite3pp.h"
#include <unistd.h>

using namespace std;
using namespace sqlite3pp;


int main(){
    
    vector<string> details (3);
    string userName;
    string passWord;
    char prompt = 'M';
    bool login;
    
    database db("user_info.db");
    db.execute("create table EMAIL( ID INTEGER PRIMARY KEY AUTOINCREMENT, Sender text, Recipient text, Subject text,  Message text, ReadTime text, writetime text)");
   
    while(toupper(prompt) != 'Q'){
        mainHead();
        cout << endl << "Register [R]        Sign-in [S]        Quit[Q]" << endl;
        cout << ": ";
        cin >> prompt; 
        if( toupper(prompt) == 'R' ){
            regis();
        }
        else if( toupper(prompt) == 'S' ){
            validation(details);
            string op = "True";
            if(details.at(0).compare(op) == 0){
            login = true;
            }
            else
                login = false;
            if( login ){
                
                bool stay = true;
                while( stay ){
                    loginHead(details.at(1));
                    
                    char retSign = sign_In();
                    
                    // For reading the inbox messages
                    if( toupper(retSign) == 'I' ){
                        bool crazyStay = true;
                        while(crazyStay){
                            char inp;
                            int unreadMessages = 0;
                            inboxHead(details.at(1));
                            unreadMessages = numUnReadMessages(details.at(1));
                            vector<string> nameOfSender = getSendernames(details.at(1));
                            inboxMessages(details.at(1), unreadMessages, nameOfSender); 
                            inp = inboxFooter();
                            if( toupper(inp) == 'M'){
                                crazyStay = false;
                            }
                            else if(toupper(inp) == 'A'){
                                string id;
                                bool amloop = true;
                                while(amloop){
                                    allMessages();
                                    displayAllTheMessages(details.at(1));
                                    cout << "ID[I]       Back[B]" << endl;
                                    cout << ": ";
                                    cin >> id;
                                    string comp = "B";
                                    string temp = id;
                                    transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
                                    
                                    if(temp.compare(comp) == 0){
                                        amloop = false;
                                    }
                                    else{

                                        int x=0;
                                        stringstream convert(id);
                                        convert >> x;
                                        // x = stoi(id);
                                        // cout << "x: " << x; 

                                        query qry(db, "SELECT id from email where id = :msgID and Recipient = :curr_user");
                                        qry.bind(":msgID", x);
                                        qry.bind(":curr_user", details.at(1), nocopy);
                                        if (qry.begin() == qry.end()) {
                                            cout << "Invalid ID" << endl;
                                            cout << "(!_!)" << endl;
                                            cout << "Press any key to continue" << endl; 
                                            cin.ignore();
                                            cin.clear();
                                            cin.sync();
                                            cin.ignore(numeric_limits<streamsize>::max(),'\n');

                                        }
                                        else{
                                            readMessage(details.at(1), x); 
                                            amloop = true;
                                            
                                        }
                                            

                                    }
                                    
                                }
 
                            }
                            else if(toupper(inp) == 'U'){

                                string unID;
                                bool unLoop = true;
                                while(unLoop){
                                    unReadMessages();
                                    displayAllTheUnreadMessages(details.at(1));
                                    cout << "ID[I]       Back[B]" << endl;
                                    cout << ": ";
                                    cin >> unID;
                                    string comp = "B";
                                    string temporary = unID;
                                    transform(temporary.begin(), temporary.end(), temporary.begin(), ::toupper);
                                    
                                    if(temporary.compare(comp) == 0){
                                        unLoop = false;
                                    }
                                    else{

                                        int y = 0;
                                        stringstream convert(unID);
                                        convert >> y;
                                        query qry(db, "SELECT id from email where id = :msgID and Recipient = :curr_user");
                                        qry.bind(":msgID", y);
                                        qry.bind(":curr_user", details.at(1), nocopy);
                                        if (qry.begin() == qry.end()) {
                                            cout << "Invalid ID" << endl;
                                            cout << "(!_!)" << endl;
                                            cout << "Press any key to continue" << endl; 
                                            cin.ignore();
                                            cin.clear();
                                            cin.sync();
                                            cin.ignore(numeric_limits<streamsize>::max(),'\n');
                                        }
                                        else{
                                            readMessage(details.at(1), y); 
                                            unLoop = true;
                                            
                                        }
                                            

                                    }
                                    
                                }
                                
                            }
                            else if(toupper(inp) == 'D'){

                                string id;
                                bool dloop = true;
                                while(dloop){
                                    deleteMessages();
                                    displayAllTheMessages(details.at(1));
                                    cout << "ID[I]       Back[B]" << endl;
                                    cout << ": ";
                                    cin >> id;
                                    string comp = "B";
                                    string temp = id;
                                    transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
                                    
                                    if(temp.compare(comp) == 0){
                                        dloop = false;
                                    }
                                    else{

                                        int x=0;
                                        stringstream convert(id);
                                        convert>>x;
                                        deleteMessage(details.at(1), x);
                                        dloop = true;
                                    }
                                }
                                
                            }
                        
                        }
                        stay = true;
                        
                    }
                    
                    // For composing the email
                    else if( toupper(retSign) == 'C' ){
                        
                        composeHead();
                        composeMessage(details.at(1));
                        stay = true;
                    }
                    
                    else if( toupper(retSign) == 'S' ){
                        settingsHead();
                        // cout << "Sorry still under construction" << endl;
                        // cout << "Wanted to add change of password feature" << endl;
                        string newPassWord;
                        cout << "Change password" << endl;
                        newPassWord = getpass("New Password: ");
                        string newPass = getHash(newPassWord);
                        cout << "Hash passWord: " << newPass << endl;
                        command cmd(db, "UPDATE USERS SET PassWord = :pow WHERE UserName = :user");
                        cmd.bind(":pow", newPass, nocopy);
                        cmd.bind(":user", details.at(1), nocopy);
                        cmd.execute(); 
                        
                        cout << "press any key to quit: " << endl;
                        cin.ignore();
                        cin.clear();
                        cin.sync();
                        cin.ignore(numeric_limits<streamsize>::max(),'\n');
                        stay = true;
                    }
                    else if( toupper(retSign) == 'L' ){
                        cout << "Loggin out......" << endl << endl;
                        stay = false;
                    }
                
                }
            }
            
        }
        else if( toupper(prompt) == 'Q' ){
            cout << "Thanks for using our services. Hope you had a good time." << endl; 
            cout << "\\m/(-_-)\\m/" << endl; 
        }
        else{
            cout << "Please choose one of the following options " << endl;
        }
        
    }
    return 0;
}