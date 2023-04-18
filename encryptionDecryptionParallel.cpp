#include "encryptionDecryptionParallel.h"
#include <iostream>
using namespace std;

// Tabella di espansione
int expansionTable2[48];
// Le substitution boxes
int substitutionBoxes2[8][4][16];

// Funzione per convertire una stringa alfanumerica in una stringa binaria
string convertStringToBinary2(string s){
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
    return bin;
}

// Funzione per convertire una stringa binaria in una stringa alfanumerica
string convertBinaryToString2(string s){
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

// Funzione per convertire una stringa binaria in un decimale
int convertBinaryToDecimalParallel(string binary){
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

// Funzione per generare le 16 chiavi in modo randomico
void generateKeysParallel(string* round_keys){
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

// Funzione per eseguire l'operazione di Xor tra due stringhe binarie
string XorParallel(string a, string b){
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

// Funzione per generare casualmente i valori della tabella di espansione e delle substitution boxes
void tablesFillerParallel(){
    for (int i = 0; i < size(expansionTable2); i++) {
        expansionTable2[i] = rand() % 32 + 1;
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 16; k++) {
                substitutionBoxes2[i][j][k] = rand() % 32;
            }
        }
    }
}

// Funzione per invertire le chiavi in modo da intercambiare tra encryption e decryption
void reverseKeysParallel(string* round_keys){
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
string DESParallel(string pt, string* round_keys){
    // Divide la stringa di plaintext in due parti uguali
    string left = pt.substr(0, 32);
    string right = pt.substr(32, 32);

    // Il plaintext viene criptato 16 volte
    for(int i=0; i<16; i++) {
        // La metà destra del plaintext viene espansa a 48 bit
        string right_expanded = "";
        for(int j : expansionTable2) {
            right_expanded += right[j - 1];
        }

        // Viene fatto lo xor con la chiave corrispondente al round di encryption
        string xored = XorParallel(round_keys[i], right_expanded);
        string res = "";

        // Il risultato viene diviso in 8 parti uguali e ognuna di esse viene passata attraverso le substitution boxes
        for(int k=0; k < 8; k++){
            // Trova l'indice della riga e della colonna da usare nella substitution box
            string row1= xored.substr(k * 6, 1) + xored.substr(k * 6 + 5, 1);
            int row = convertBinaryToDecimalParallel(row1);
            string col1 = xored.substr(k * 6 + 1, 1) + xored.substr(k * 6 + 2, 1) +
                    xored.substr(k * 6 + 3, 1) + xored.substr(k * 6 + 4, 1);;
            int col = convertBinaryToDecimalParallel(col1);
            int val = substitutionBoxes2[k][row][col];
            res += convertDecimalToBinaryParallel(val);
        }

        //Il risultato della sostituzione viene messo in xor con la parte di sinistra del plaintext originale
        xored = XorParallel(res, left);
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

// Decriptazione parallela
bool parallelDecryption(vector<string> lines, int size, int nThreads){
    bool isCorrect = true;
    string round_keys[16];
    tablesFillerParallel();
    omp_set_num_threads(nThreads);
#pragma omp parallel private(round_keys) shared(expansionTable2, substitutionBoxes2)
    {
        generateKeysParallel(round_keys);
        reverseKeysParallel(round_keys);
#pragma omp for
        for (int j = 0; j < size; j++) {
            reverseKeysParallel(round_keys);
            string pt = convertStringToBinary2(lines[j]);
            string ct = DESParallel(pt, round_keys);
            reverseKeysParallel(round_keys);
            string decrypted = DESParallel(ct, round_keys);
            string x = convertBinaryToString2(decrypted);
            if (x != lines[j]) {
                cout << x << "-" << lines[j] << endl;
                cout << "DECRIPTAZIONE FALLITA" << endl;
                isCorrect = false;
            }
        }
    }
    return isCorrect;
}
