#include <iostream>
#include <string.h>

using namespace std;

string getHash(string String);
string encrypt(string inputMessage, string passPhrase);
string decrypt(string message, string passPhrase);
// string decrypt(string ciphertextStr, string passPhrase);
// string* encrypt(string plaintext, string passphrase);