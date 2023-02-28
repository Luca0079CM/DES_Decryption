#include "encryptionDecryptionSequential.h"
#include <string>
#include <cmath>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

// L'array delle chiavi
string round_keys[16];
// Tabella di espansione
int expansionTable[48];
// Le substitution boxes
int substitutionBoxes[8][4][16];

string convertStringToBinary(string s){
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

string convertBinaryToString(string s){
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

// Funzione per convertire un numero in una stringa binaria
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

// Funzione per convertire una stringa binaria in un decimale
int convertBinaryToDecimal(string binary){
    int decimal = 0;
    int counter = 0;
    for(int i = binary.length()-1; i >= 0; i--){
        if(binary[i] == '1'){
            decimal += pow(2, counter);
        }
        counter++;
    }
    return decimal;
}

// Funzione per eseguire l'operazione di Xor tra due stringhe binarie
string Xor(string a, string b){
    string result = "";
    for(int i = 0; i < b.size(); i++){
        if(a[i] != b[i]){
            result += "1";
        }
        else{
            result += "0";
        }
    }
    return result;
}

// Funzione per generare le 16 chiavi in modo randomico
void generateKeys(){
    for(int i=0; i<16; i++){
        string round_key = "";
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

// Funzione per generare casualmente i valori della tabella di espansione e delle substitution boxes
void tablesFiller(){
    for(int & i : expansionTable){
        i = rand()%32 + 1;
    }

    for(int i=0; i<8; i++){
        for(int j=0; j<4; j++){
            for(int k=0; k<16; k++){
                substitutionBoxes[i][j][k] = rand() % 32;
            }
        }
    }
}

// Funzione per invertire le chiavi in modo da intercambiare tra encryption e decryption
void reverseKeys(){
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

// Implementazione dell'algoritmo DES
string DES(string pt){
    // Divide la stringa di plaintext in due parti uguali
    string left = pt.substr(0, 32);
    string right = pt.substr(32, 32);

    // Il plaintext viene criptato 16 volte
    for(int i=0; i<16; i++) {
        // La metà destra del plaintext viene espansa a 48 bit
        string right_expanded = "";
        for(int j : expansionTable) {
            right_expanded += right[j - 1];
        }

        // Viene fatto lo xor con la chiave corrispondente al round di encryption
        string xored = Xor(round_keys[i], right_expanded);
        string res = "";
        // Il risultato viene diviso in 8 parti uguali e ognuna di esse viene passata attraverso le substitution boxes
        for(int k=0; k < 8; k++){
            // Trova l'indice della riga e della colonna da usare nella substitution box
            string row1= xored.substr(k * 6, 1) + xored.substr(k * 6 + 5, 1);
            int row = convertBinaryToDecimal(row1);
            string col1 = xored.substr(k * 6 + 1, 1) + xored.substr(k * 6 + 2, 1) +
                    xored.substr(k * 6 + 3, 1) + xored.substr(k * 6 + 4, 1);;
            int col = convertBinaryToDecimal(col1);
            int val = substitutionBoxes[k][row][col];
            res += convertDecimalToBinary(val);
        }

        //Il risultato della sostituzione viene messo in xor con la parte di sinistra del plaintext originale
        xored = Xor(res, left);
        // Vengono scambiate la parte sinistra e destra per prepararsi al nuovo round
        left = xored;
        // Tranne che all'ultimo round
        if(i < 15){
            string temp = right;
            right = xored;
            left = temp;
        }
    }

    // Vengono rimesse insieme le due parti
    string ciphertext = left + right;

    return ciphertext;
}

void sequentialDecryption(vector<string> lines, int size){
    int i = 0;
    generateKeys();
    reverseKeys();
    tablesFiller();
    for (int j = 0; j < size; j++) {
        reverseKeys();
        i++;
        string pt = convertStringToBinary(lines[j]);
        // Calling the function to generate 16 keys
        // Applying the algo
        string ct = DES(pt);
        reverseKeys();
        pt = ct;
        string decrypted = DES(pt);
        // Comparing the initial plain text with the decrypted text
        string x = convertBinaryToString(decrypted);
        if (x != lines[j]){
            cout<<"DECRIPTAZIONE FALLITA"<<endl;
            break;
        }
        if (i % 10000 == 0)
            cout<<"Eseguite "<<i<<" decriptazioni"<<endl;
    }

}
