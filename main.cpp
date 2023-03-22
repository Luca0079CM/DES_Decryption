#include <iostream>
#include <string>
#include <chrono>
#include <omp.h>
#include "encryptionDecryptionSequential.h"
#include "encryptionDecryptionParallel.h"

#define nLines 10000
#define testLines 5000
#define nTest 5

using namespace std;

int main() {
    ifstream file("password.txt");
    // Salva le righe del file in un vettore, per permettere l'elaborazione in modo parallelo
    vector<string> lines(nLines);
    for (int j = 0; j < nLines; ++j)
        getline(file, lines[j]);

    // 1) Test all'aumentare del numero di Thread con 5000 password
    // Decriptazione sequenziale
    int sequentialTime = 0;
    for(int i = 0; i<nTest; i++) {
        auto start = chrono::system_clock::now();
        if(!sequentialDecryption(lines, testLines)){
            break;
        }
        auto end = chrono::system_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
        sequentialTime += elapsed.count();
    }
    cout << "Tempo Decriptazione Sequenziale: " << sequentialTime/nTest <<endl<<endl;

    // Decriptazione Parallela
    int maxThreads = omp_get_max_threads();
    for (int nThreads = 2; nThreads < 4*maxThreads+1; nThreads++) {
        int parallelTime = 0;
        for (int i = 0; i < nTest; i++) {
            auto start = chrono::system_clock::now();
            if(!parallelDecryption(lines, testLines, nThreads)){
                break;
            }
            auto end = chrono::system_clock::now();
            auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
            parallelTime += elapsed.count();
        }
        cout << "Tempo Decriptazione Parallela usando " << nThreads << " threads: " << parallelTime / nTest << endl;
        float speedup = (float) sequentialTime/parallelTime;
        cout<<"SpeedUp: "<<speedup<<endl<<endl;
    }

    // 2)Test all'aumentare del numero di password
    // Decriptazione Sequenziale
    for(int n=testLines; n < nLines+1000; n+=1000) {
        auto start = chrono::system_clock::now();
        if(!sequentialDecryption(lines, n)){
            break;
        }
        auto end = chrono::system_clock::now();
        auto elapsed = chrono::duration_cast<std::chrono::milliseconds>(end - start);
        cout << "Tempo Decriptazione Sequenziale con "<<n<<" password: "<<elapsed.count()<<endl;
    }

    // Decriptazione Parallela
    for (int n=testLines; n < nLines+1000; n+=1000) {
        auto start = chrono::system_clock::now();
        if(!parallelDecryption(lines, n, maxThreads)){
            break;
        }
        auto end = chrono::system_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << "Tempo Decriptazione Parallela con " << n << " password: " << elapsed.count() << endl;
    }
}