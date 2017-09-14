#include <iostream>
#include <string.h>
#include <algorithm>
#include <cstdlib>
#include <cryptopp/cryptlib.h>
#include <cryptopp/salsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <stdio.h>

using namespace std;
using namespace CryptoPP;

string getHash(string String){
    SHA256 hash;
    string message = String;
    string hash_Out;
    StringSource s(message, true, new HashFilter(hash, new HexEncoder(new StringSink(hash_Out))));
    
    return hash_Out;
}

string encrypt(string plaintext, string passphrase) {
  AutoSeededRandomPool prng;
  string cdata = "";
  string cipher= "";
  string encoded;
  string recovered;

  string passPhrase = getHash(passphrase);
  
  byte key[32];
  memcpy(key, passPhrase.c_str(), 32);
  
  byte iv[8];
  prng.GenerateBlock(iv, sizeof(iv));

  // Pretty print key
  encoded.clear();
  StringSource(key, sizeof(key), true, new HexEncoder(new StringSink(encoded)));
  string key_str = encoded;

  // Pretty print iv
  encoded.clear();
  StringSource(iv, sizeof(iv), true, new HexEncoder(new StringSink(encoded)));
  string iv_str = encoded;

  try {
    Salsa20::Decryption salsa;
    salsa.SetKeyWithIV(key, 32, iv);

    StringSource ss(plaintext, true, new StreamTransformationFilter(salsa, new StringSink(cipher)));
      
  }
  catch (Exception& e){
    std::cerr << e.what() << std::endl;
    exit(1);
      
  }

  encoded.clear();
  StringSource(cipher, true, new HexEncoder(new StringSink(encoded)));    
  string ciphertext = encoded;
	ciphertext = iv_str + ciphertext;	
  return ciphertext;
  
}

string decrypt(string ciphertextStr, string passPhrase) {
  
  string plaintext = "";
  string encoded;

	string iv_str = ciphertextStr.substr(0,16);
	string ciphertext = ciphertextStr.substr(16);
	
  string passphrase = getHash(passPhrase);
  
  byte key[32];
  memcpy(key, passphrase.c_str(), 32);
  
  encoded.clear();
  StringSource(key, sizeof(key), true, new HexEncoder(new StringSink(encoded)));
  cout << "key: " << encoded << endl;
  string key_str = encoded;

  byte iv[8];
  HexDecoder decoder;
  decoder.Put((byte*)iv_str.data(), iv_str.size());
  decoder.MessageEnd();
  decoder.Get(iv, sizeof(iv));
  
  key[32];
  {
    HexDecoder decoder;
    decoder.Put((byte*)key_str.data(), key_str.size());
    decoder.MessageEnd();
    decoder.Get(key, sizeof(key));      
  }

  string cipher_raw;
  {
    HexDecoder decoder;
    decoder.Put((byte*)ciphertext.data(), ciphertext.size());
    decoder.MessageEnd();

    long long size = decoder.MaxRetrievable();
    cipher_raw.resize(size);
    decoder.Get((byte*)cipher_raw.data(), cipher_raw.size());
      
  }
  // Decryption
  try {
    Salsa20::Decryption salsa;
    salsa.SetKeyWithIV(key, 32, iv);
    
    StringSource ss(cipher_raw, true, new StreamTransformationFilter(salsa, new StringSink(plaintext)));
  }
  catch (Exception& e){
    std::cerr << e.what() << std::endl;
    exit(1);
  }
  
  return plaintext;  
}

