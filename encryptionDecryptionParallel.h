#ifndef DES_DECRYPTION_ENCRYPTIONDECRYPTIONPARALLEL_H
#define DES_DECRYPTION_ENCRYPTIONDECRYPTIONPARALLEL_H
#include <omp.h>
#include <fstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

string convertDecimalToBinaryParallel(int decimal);
int convertBinaryToDecimalParallel(string binary);
string XorParallel(string a, string b);
void generateKeysParallel(string* round_keys);
void tablesFillerParallel();
string DESParallel(string plaintext, string* round_keys);
void reverseKeysParallel(string* round_keys);
bool parallelDecryption(vector<string> lines, int size, int nThreads);

#endif //DES_DECRYPTION_ENCRYPTIONDECRYPTIONPARALLEL_H
