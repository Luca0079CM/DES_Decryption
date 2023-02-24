#include "encryptionDecryptionParallel.h"
#include <iostream>
using namespace std;

// The expansion table
int expansionTable2[48];
// The substitution boxes. They should contain values from 0 to 15 in any order.
int substitutionBoxes2[8][4][16];

string convertStringToBinary2(string s){
    // hexadecimal to binary conversion
    unordered_map<char, string> mp;
    mp['0'] = "00000000";
    mp['1'] = "00000001";
    mp['2'] = "00000010";
    mp['3'] = "00000011";
    mp['4'] = "00000100";
    mp['5'] = "00000101";
    mp['6'] = "00000110";
    mp['7'] = "00000111";

    mp['8'] = "00001000";
    mp['9'] = "00001001";
    mp['A'] = "00001010";
    mp['B'] = "00001011";
    mp['C'] = "00001100";
    mp['D'] = "00001101";
    mp['E'] = "00001110";
    mp['F'] = "00001111";

    mp['G'] = "00010000";
    mp['H'] = "00010001";
    mp['I'] = "00010010";
    mp['J'] = "00010011";
    mp['K'] = "00010100";
    mp['L'] = "00010101";
    mp['M'] = "00010110";
    mp['N'] = "00010111";

    mp['O'] = "00011000";
    mp['P'] = "00011001";
    mp['Q'] = "00011010";
    mp['R'] = "00011011";
    mp['S'] = "00011100";
    mp['T'] = "00011101";
    mp['U'] = "00011110";
    mp['V'] = "00011111";

    mp['W'] = "00100001";
    mp['X'] = "00100010";
    mp['Y'] = "00100011";
    mp['Z'] = "00100100";

    string bin = "";
    for (char i : s) {
        bin += mp[i];
    }
    //cout<<"Size: "<<size(bin)<<"\n";
    return bin;
}

string convertBinaryToString2(string s){
    // binary to string conversion
    unordered_map<string, string> mp;
    mp["00000000"] = "0";
    mp["00000001"] = "1";
    mp["00000010"] = "2";
    mp["00000011"] = "3";
    mp["00000100"] = "4";
    mp["00000101"] = "5";
    mp["00000110"] = "6";
    mp["00000111"] = "7";

    mp["00001000"] = "8";
    mp["00001001"] = "9";
    mp["00001010"] = "A";
    mp["00001011"] = "B";
    mp["00001100"] = "C";
    mp["00001101"] = "D";
    mp["00001110"] = "E";
    mp["00001111"] = "F";

    mp["00010000"] = "G";
    mp["00010001"] = "H";
    mp["00010010"] = "I";
    mp["00010011"] = "J";
    mp["00010100"] = "K";
    mp["00010101"] = "L";
    mp["00010110"] = "M";
    mp["00010111"] = "N";

    mp["00011000"] = "O";
    mp["00011001"] = "P";
    mp["00011010"] = "Q";
    mp["00011011"] = "R";
    mp["00011100"] = "S";
    mp["00011101"] = "T";
    mp["00011110"] = "U";
    mp["00011111"] = "V";

    mp["00100001"] = "W";
    mp["00100010"] = "X";
    mp["00100011"] = "Y";
    mp["00100100"] = "Z";

    string hex = "";
    for (int i = 0; i < s.length(); i += 8) {
        string ch = "";
        ch += s[i];
        ch += s[i + 1];
        ch += s[i + 2];
        ch += s[i + 3];
        ch += s[i + 4];
        ch += s[i + 5];
        ch += s[i + 6];
        ch += s[i + 7];
        hex += mp[ch];
    }
    return hex;
}

// Function to convert a number in decimal to binary
string convertDecimalToBinaryParallel(int decimal){
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
int convertBinaryToDecimalParallel(string binary){
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

// Function to generate the 16 keys.
void generateKeysParallel(string* round_keys){
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

// Function to compute xor between two strings
string XorParallel(string a, string b){
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

void tablesFillerParallel(){
    for (int i = 0; i < size(expansionTable2); i++) {
        expansionTable2[i] = rand() % 32 + 1;
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 16; k++) {
                substitutionBoxes2[i][j][k] = rand() % 16;
            }
        }
    }
}

void reverseKeysParallel(string* round_keys){
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
string DESParallel(string pt, string* round_keys){
    // 2. Dividing the result into two equal halves
    string left = pt.substr(0, 32);
    string right = pt.substr(32, 32);
    // The plain text is encrypted 16 times
    for(int i=0; i<16; i++) {
        string right_expanded = "";
        // 3.1. The right half of the plain text is expanded
        for(int j : expansionTable2) {
            right_expanded += right[j - 1];
        }
        // 3.3. The result is xored with a key
        string xored = XorParallel(round_keys[i], right_expanded);
        string res = "";
        // 3.4. The result is divided into 8 equal parts and passed
        // through 8 substitution boxes. After passing through a
        // substituion box, each box is reduces from 6 to 4 bits.
        for(int k=0; k < 8; k++){
            // Finding row and column indices to look up the
            // substituition box
            string row1= xored.substr(k * 6, 1) + xored.substr(k * 6 + 5, 1);
            int row = convertBinaryToDecimalParallel(row1);
            string col1 = xored.substr(k * 6 + 1, 1) + xored.substr(k * 6 + 2, 1) + xored.substr(k * 6 + 3, 1) + xored.substr(k * 6 + 4, 1);;
            int col = convertBinaryToDecimalParallel(col1);
            int val = substitutionBoxes2[k][row][col];
            res += convertDecimalToBinaryParallel(val);
        }
        // 3.5. The result is xored with the left half
        xored = XorParallel(res, left);
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

void parallelDecryption(vector<string> lines){
    int i = 0;
    int tid;
    tid = -1;
    string round_keys[16];

    tablesFillerParallel();
#pragma omp parallel private(round_keys, tid) shared(expansionTable2, substitutionBoxes2)
    {
        tid = omp_get_thread_num();
        cout << "Hello from thread: " << tid << endl;
        generateKeysParallel(round_keys);
        reverseKeysParallel(round_keys);
#pragma omp for
        for (int j = 0; j < size(lines); j++) {
            i++;
            reverseKeysParallel(round_keys);
            string pt = convertStringToBinary2(lines[j]);
            // Applying the algo
            string ct = DESParallel(pt, round_keys);
            reverseKeysParallel(round_keys);
            pt = ct;
            string decrypted = DESParallel(pt, round_keys);
            // Comparing the initial plain text with the decrypted text
            string x = convertBinaryToString2(decrypted);
            if (x != lines[j]) {
                cout << x << "-" << lines[j] << endl;
                cout << "DECRIPTAZIONE FALLITA" << endl;
                // CE UN MODO PER FERMARE TUTTI I THREAD??
            }
            if (i % 10000 == 0)
                cout << "Eseguite " << i << " decriptazioni" << endl;
        }
    }
}

void prova(){
    int V[500000];
#pragma omp parallel for shared(V)
    for(int i = 0; i < size(V); i++){
        V[i] = i+1;
    }

    for(int i = 0; i < size(V); i++){
        cout<<V[i]<<"; ";
    }
    cout<<"-"<<size(V);
}
