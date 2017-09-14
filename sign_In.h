#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

char sign_In();
void inboxMessages(string user_curr, int unreadMessages , vector<string> listSenders);
void composeMessage(string senderName);
int numUnReadMessages(string user_curr);
int numUserUnReadMessages(string user_curr, string senderName);
vector<string> getSendernames(string user_curr);
vector<string> getAllMessages(string usER);
vector<string> getAllUnReadMessages(string user_curr);
char inboxFooter();
void displayAllTheMessages(string user_curr);
void deleteMessage(string curr_user, int id);
void readMessage(string curr_user, int id);
void displayAllTheUnreadMessages(string user_curr);
void updatetime(string curr_user, int id);
