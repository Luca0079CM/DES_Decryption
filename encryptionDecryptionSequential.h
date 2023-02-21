#ifndef DES_DECRYPTION_ENCRYPTIONDECRYPTIONSEQUENTIAL_H
#define DES_DECRYPTION_ENCRYPTIONDECRYPTIONSEQUENTIAL_H
#include <string>
using namespace std;

string convertDecimalToBinary(int decimal);
int convertBinaryToDecimal(string binary);
string Xor(string a, string b);
void generateKeys();
void tablesFiller();
string DES(string plaintext);
void reverseKeys();

#endif //DES_DECRYPTION_ENCRYPTIONDECRYPTIONSEQUENTIAL_H
