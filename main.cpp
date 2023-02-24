#include <iostream>
#include <string>
#include <bits/stdc++.h>
#include <chrono>
#include <omp.h>
#include "encryptionDecryptionSequential.h"
#include "encryptionDecryptionParallel.h"

using namespace std;

void mainProva();

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

int main(){
    ifstream file("password.txt");

    // Decriptazione sequenziale
    int i = 0;
    auto start = std::chrono::system_clock::now();

    if (file.is_open()) {
        generateKeys();
        reverseKeys();
        tablesFiller();
        for (string line; getline(file, line);) {
            reverseKeys();
            i++;
            string pt = convertStringToBinary(line);
            // Calling the function to generate 16 keys
            // Applying the algo
            string ct = DES(pt);
            reverseKeys();
            pt = ct;
            string decrypted = DES(pt);
            // Comparing the initial plain text with the decrypted text
            string x = convertBinaryToString(decrypted);
            if (x != line){
                cout<<"DECRIPTAZIONE FALLITA"<<endl;
                break;
            }
            if (i % 10000 == 0)
                cout<<"Eseguite "<<i<<" decriptazioni"<<endl;
        }
    }

    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    int sequentialTime = elapsed.count();
    cout<<"Tempo Decriptazione Sequenziale: "<< sequentialTime<<endl;
    file.close();

    // Decriptazione Parallela
    ifstream file2("password.txt");
    i = 0;

    start = std::chrono::system_clock::now();
    // Save row of file for storing in a vector
    int n_lines = 10000;
    vector<string> lines(n_lines);
    for (int j = 0; j < n_lines; ++j )
        std::getline(file2, lines[j]);
    file2.close();

    parallelDecryption(lines);

    end = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    int parallelTime = elapsed.count();
    cout<<"Tempo Decriptazione Parallela: "<< parallelTime<<endl;
    float speedup = sequentialTime/parallelTime;
    cout<<"SpeedUp: "<<speedup<<endl;
    cout<<"Differenza Tempi:"<<sequentialTime-parallelTime<<endl;
    file2.close();
}

void mainProva(){
    auto start = std::chrono::system_clock::now();
    prova();
    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    int sequentialTime = elapsed.count();
    cout<<endl;
    cout<<"Tempo: "<< sequentialTime<<endl;
}