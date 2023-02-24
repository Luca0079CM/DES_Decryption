#ifndef DES_DECRYPTION_ENCRYPTIONDECRYPTIONPARALLEL_H
#define DES_DECRYPTION_ENCRYPTIONDECRYPTIONPARALLEL_H

#include <opencv2/opencv.hpp>
#include <omp.h>
#include <iostream>
#include <string>
using namespace std;
using namespace cv;

string convertDecimalToBinaryParallel(int decimal);
int convertBinaryToDecimalParallel(string binary);
string XorParallel(string a, string b);
void generateKeysParallel(string* round_keys);
void tablesFillerParallel();
string DESParallel(string plaintext, string* round_keys);
void reverseKeysParallel(string* round_keys);
void parallelDecryption(vector<string> lines);
void prova();


#endif //DES_DECRYPTION_ENCRYPTIONDECRYPTIONPARALLEL_H
