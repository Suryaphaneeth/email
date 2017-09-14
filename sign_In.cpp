#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <list>
#include <algorithm>
#include "head.h"
#include "crypt.h"
#include "sqlite3pp/sqlite3pp.h"

using namespace std;
using namespace sqlite3pp;
// database db("user_info.db");


char sign_In(){
    
    char logPrompt = 'U';
    
    cout << "Inbox[I]       Compose[C]      Settings[S]      Log-out[L]" << endl;
    cout << ": ";
    cin >> logPrompt;
    while( !(toupper(logPrompt) == 'I' || toupper(logPrompt) == 'C' || toupper(logPrompt) == 'S' || toupper(logPrompt) == 'L') ){
        cout << endl << "Please choose from the following options: " << endl;
        cout << ": ";
        cin >> logPrompt;
        cout << "Inbox[I]       Compose[C]      Settings[S]      Log-out[L]" << endl;
    }
    
    return logPrompt;
    
}

int numUnReadMessages(string user_curr){
    
    int counter;
    database db("user_info.db");
    query qry(db, "SELECT count(*) from EMAIL where Recipient = :senderName AND ReadTime = :readTime ");
    // query qry(db, "SELECT count(*) from EMAIL where Recipient = :senderName ");
    qry.bind(":senderName", user_curr, nocopy);
    qry.bind(":readTime", "", nocopy);
    for (auto v : qry) {
    string tmpCount = "";
    v.getter() >> tmpCount;
    counter = stoi(tmpCount);
  }
  
    return counter;
}

int numUserUnReadMessages(string user_curr, string senderName){
    
    int counter;
    database db("user_info.db");
    query qry(db, "SELECT count(*) from EMAIL where Recipient = :recepName AND Sender = :sendName AND ReadTime = :readTime ");
    qry.bind(":recepName", user_curr, nocopy);
    qry.bind(":sendName", senderName, nocopy);
    qry.bind(":readTime", "", nocopy);
    for (auto v : qry) {
    string tmpCount = "";
    v.getter() >> tmpCount;
    counter = stoi(tmpCount);
    }

    return counter;
}

vector<string> getAllMessages(string usER){
    
    vector<string> messList;
    database db("user_info.db");
    query qry(db, "SELECT ID, sender, subject, writetime from EMAIL where Recipient = :presUser");
    qry.bind(":presUser", usER, nocopy);

    for (auto v : qry) {
        string id = "";
        string sender = "";
        string subject = "";
        string writetime = "";
//        string readtime = "";
        v.getter() >> id >>  sender >> subject >> writetime;
        messList.push_back( id + " | " + sender + " | " + subject + " | " + writetime + " | " );
        
    }

    return messList;
    
}

void inboxMessages(string user_curr, int unreadMessages, vector<string> listSenders){
    
    string curUser;
    int count ;
    curUser = user_curr;
    cout << curUser << ", you have " << unreadMessages << " unread messages." << endl;
    
    for(int i = 0 ; i < listSenders.size() ; i++){
        int num = numUserUnReadMessages(user_curr, listSenders.at(i));
        cout << "User: " ;
        cout << listSenders.at(i) << " - " << num << endl;
    }
    
    database db("user_info.db");
    
    cout << endl << "-----------------------------------------------------MESSAGES-----------------------------------------------------" << endl;
    
    vector<string> myMess = getAllMessages(user_curr);
    for(int i = 0 ; i < myMess.size() ; i++){

        cout << myMess.at(i) << endl;
    }
    
    
    cout << endl;
    
}

void composeMessage(string senderName){
    
    string sender;
    string receiver;
    string subject;
    string signature;
    string message;
    string encryptMessage;
    bool choice = true;
    string writeTime;
    
    database db("user_info.db");
    sender = senderName;
    while(choice){
        
        cout << "To: ";
        cin >> receiver;
        cin.ignore();
        query qry(db, "SELECT UserName from USERS where UserName = :username");
        qry.bind(":username", receiver, nocopy);
        if (qry.begin() == qry.end()) { 
            cout << endl << "(O_o)                                  (o_O)" << endl;
            cout << "Seems like your friend isn't there on YEES. Dayum!! he is missing all the fun. Invite him." << endl;
            //cout << "" << endl;
            choice = true;
        }
        else {
            choice = false;
        }
    }
    
    clearScreen();
    composeHead();
    
    cout << "To: " << receiver << endl;
    cout << "Subject: ";
    cin.clear();
    getline(cin, subject);
    cin.clear();
    cout << "Message: ";
    getline(cin, message);
    cin.clear();
    cout << "Signature: ";
    cin >> signature;
   // signature = getHash( signature );
    writeTime = getTime();
    
    message = encrypt(message, signature);
    
    //database db("user_info.db");
    command cmd(db, "INSERT INTO EMAIL (Sender, Recipient, Subject, Message, ReadTime, writetime) VALUES (?,?,?,?,?,?)");
    cmd.binder() << sender << receiver << subject << message << "" << writeTime;
    cmd.execute();
    cout << "Successfully sent" << endl << endl;

}


vector<string> getSendernames(string user_curr){
        
    vector<string> listUsers;
    database db("user_info.db");
    query qry(db, "SELECT sender from EMAIL where Recipient = :senderName AND ReadTime = :readTime ");
    qry.bind(":senderName", user_curr, nocopy);
    qry.bind(":readTime", "", nocopy);

    for (auto v : qry) {
        string name;
        v.getter() >> name;
        listUsers.push_back(name);
        
    }
    sort( listUsers.begin(), listUsers.end() );
    listUsers.erase( unique( listUsers.begin(), listUsers.end() ), listUsers.end() );
  
    return listUsers;
}

char inboxFooter(){
    
    bool check = true;
    char footerValue;
    
    while (check){
        
        cout << "All Messages[A]      Unread Messages[U]        Delete Messages[D]      Main Menu[M]" << endl;
        cout <<": ";
        cin >> footerValue;
        if( toupper(footerValue) == 'A' || toupper(footerValue) == 'U' || toupper(footerValue) == 'D' ||  toupper(footerValue) == 'M'  ){
            check = false;
        }
        
    }
    return footerValue;
}

void displayAllTheMessages(string user_curr){
    string val;
    vector<string> myMess = getAllMessages(user_curr);
    for(int i = 0 ; i < myMess.size() ; i++){

        cout << myMess.at(i) << endl;
    }
}

void deleteMessage(string curr_user, int id){\
    database db("user_info.db");
    command cmd(db, "DELETE FROM EMAIL WHERE Recipient = :curr_user AND ID = :id");
    cmd.binder() << curr_user << id;
    cmd.execute();
    cout << "Deleted message " << id << ".";
}

vector<string> getAllUnReadMessages(string usER){
    
    vector<string> messList;
    database db("user_info.db");
    query qry(db, "SELECT ID, sender, subject, writetime from EMAIL where Recipient = :presUser AND ReadTime = :readtime ");
    qry.bind(":presUser", usER, nocopy);
    qry.bind(":readtime", "", nocopy);

    for (auto v : qry) {
        string id = "";
        string sender = "";
        string subject = "";
        string writetime = "";
        v.getter() >> id >>  sender >> subject >> writetime;
        messList.push_back( id + " | " + sender + " | " + subject + " | " + writetime + " | " );
        
    }
    return messList;
    
}


void displayAllTheUnreadMessages(string user_curr){
    string val;
    vector<string> myMess = getAllUnReadMessages(user_curr);
    for(int i = 0 ; i < myMess.size() ; i++){
        cout << myMess.at(i) << endl;
    }
}

void updatetime(string curr_user, int id){
    database db("user_info.db");

    string readtime = getTime();
    
  
     command cmd(db, "UPDATE email SET readtime = :read WHERE id = :id AND recipient = :curr_user");
     cmd.bind(":read", readtime, nocopy);
     cmd.bind(":id", to_string(id), nocopy);
     cmd.bind(":curr_user", curr_user, nocopy);
     cmd.execute(); 
}

void readMessage(string curr_user, int id){
     database db("user_info.db");
     string readtime;
     string message;
     string writeTime;
     string decyptedMessage;
     string encrypedMessage;
     string signature;
     string sender;
     string passPhrase;
     string subject;

     query qry(db, "SELECT sender, Subject, message, writetime FROM EMAIL WHERE ID = :msgid AND Recipient = :curr_user " );
     qry.bind(":msgid", id);
     qry.bind(":curr_user", curr_user, nocopy);
     for (auto v : qry) {
        encrypedMessage = "";
        sender = "" ;
        writeTime = "" ;
        subject = "";
        v.getter() >>sender >> subject >> encrypedMessage >> writeTime;
     }
     cout << "Signature: ";
     cin >> passPhrase;
     decyptedMessage = decrypt(encrypedMessage, passPhrase);

     updatetime(curr_user, id);

    //  command cmd( db, "INSERT INTO EMAIL (ReadTime) VALUES (:time)");
    // cmd.bind(":time", readtime, nocopy);
    // cmd.execute();
      clearScreen();

     cout << "From: " << sender << endl;
     cout << "Subject: " << subject << endl;
     cout << "Message: " << decyptedMessage << endl;
     cout << "Time Received: " << writeTime<< endl;
     cin.ignore();
     cin.clear();
     cin.sync();
     cin.ignore(numeric_limits<streamsize>::max(),'\n');

}

