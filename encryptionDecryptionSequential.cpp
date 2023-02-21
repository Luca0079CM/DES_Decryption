#include "encryptionDecryptionSequential.h"
#include <string>
#include <cmath>
using namespace std;

// Array to hold 16 keys
string round_keys[16];
// The expansion table
int expansionTable[48];
// The substitution boxes. They should contain values from 0 to 15 in any order.
int substitutionBoxes[8][4][16];

// Function to convert a number in decimal to binary
string convertDecimalToBinary(int decimal){
    string binary;
    while(decimal != 0) {
        binary = (decimal % 2 == 0 ? "0" : "1") + binary;
        decimal = decimal/2;
    }
    while(binary.length() < 4){
        binary = "0" + binary;
    }
    return binary;
}
// Function to convert a number in binary to decimal
int convertBinaryToDecimal(string binary){
    int decimal = 0;
    int counter = 0;
    int size = binary.length();
    for(int i = size-1; i >= 0; i--){
        if(binary[i] == '1'){
            decimal += pow(2, counter);
        }
        counter++;
    }
    return decimal;
}

// Function to compute xor between two strings
string Xor(string a, string b){
    string result = "";
    int size = b.size();
    for(int i = 0; i < size; i++){
        if(a[i] != b[i]){
            result += "1";
        }
        else{
            result += "0";
        }
    }
    return result;
}
// Function to generate the 16 keys.
void generateKeys(){
    for(int i=0; i<16; i++){
        string round_key = "";
        // Finally, using the PC2 table to transpose the key bits
        srand((unsigned)time(NULL) * (i+1));
        int random;
        for(int j = 0; j < 48; j++){
            random = rand();
            if((random - j) % 2 == 0){
                round_key += "0";
            }
            else{
                round_key += "1";
            }
        }
        round_keys[i] = round_key;
    }
}

void tablesFiller(){
    for(int & i : expansionTable){
        i = rand()%32 + 1;
    }

    for(int i=0; i<8; i++){
        for(int j=0; j<4; j++){
            for(int k=0; k<16; k++){
                substitutionBoxes[i][j][k] = rand() % 16;
            }
        }
    }
}

void reverseKeys(){
    // Reversing the round_keys array for decryption
    int i = 15;
    int j = 0;
    while(i > j){
        string temp = round_keys[i];
        round_keys[i] = round_keys[j];
        round_keys[j] = temp;
        i--;
        j++;
    }
}

// Implementing the algorithm
string DES(string pt){
    // 2. Dividing the result into two equal halves
    string left = pt.substr(0, 32);
    string right = pt.substr(32, 32);
    // The plain text is encrypted 16 times
    for(int i=0; i<16; i++) {
        string right_expanded = "";
        // 3.1. The right half of the plain text is expanded
        for(int j : expansionTable) {
            right_expanded += right[j - 1];
        }
        // 3.3. The result is xored with a key
        string xored = Xor(round_keys[i], right_expanded);
        string res = "";
        // 3.4. The result is divided into 8 equal parts and passed
        // through 8 substitution boxes. After passing through a
        // substituion box, each box is reduces from 6 to 4 bits.
        for(int k=0; k < 8; k++){
            // Finding row and column indices to look up the
            // substituition box
            string row1= xored.substr(k * 6, 1) + xored.substr(k * 6 + 5, 1);
            int row = convertBinaryToDecimal(row1);
            string col1 = xored.substr(k * 6 + 1, 1) + xored.substr(k * 6 + 2, 1) + xored.substr(k * 6 + 3, 1) + xored.substr(k * 6 + 4, 1);;
            int col = convertBinaryToDecimal(col1);
            int val = substitutionBoxes[k][row][col];
            res += convertDecimalToBinary(val);
        }
        // 3.5. The result is xored with the left half
        xored = Xor(res, left);
        // 3.6. The left and the right parts of the plain text are swapped
        left = xored;
        if(i < 15){
            string temp = right;
            right = xored;
            left = temp;
        }
    }

    // 4. The halves of the plain text are applied
    string ciphertext = left + right;

    return ciphertext;
}
