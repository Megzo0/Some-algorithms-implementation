#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

// 1. Hashing Techniques

// Division Method
int divisionMethod(int key, int tableSize) {
    return key % tableSize;
}

// Multiplication Method
int multiplicationMethod(int key, int tableSize) {
    const double A = 0.618033; // Fractional part of the golden ratio
    double fractionalPart = key * A - floor(key * A);
    return static_cast<int>(floor(tableSize * fractionalPart));
}

// Mid Square Method
int midSquareMethod(int key, int tableSize) {
    int squared = key * key;
    string squaredStr = to_string(squared);
    int mid = squaredStr.size() / 2;
    int midDigits = stoi(squaredStr.substr(mid - 1, 2)); // Extract middle digits
    return midDigits % tableSize;
}

// Folding Method
int foldingMethod(int key, int tableSize) {
    string keyStr = to_string(key);
    int sum = 0;
    for (size_t i = 0; i < keyStr.size(); i += 2) {
        int part = stoi(keyStr.substr(i, 2)); // Take two digits at a time
        sum += part;
    }
    return sum % tableSize;
}

// 2. Collision Resolution Techniques

// Chaining Method
void chainingMethod(vector<vector<int>>& hashTable, int key, int tableSize) {
    int index = divisionMethod(key, tableSize);
    hashTable[index].push_back(key);
}

// Open Addressing Method (Linear Probing)
int openAddressingLinear(vector<int>& hashTable, int key, int tableSize) {
    int index = divisionMethod(key, tableSize);
    int originalIndex = index;
    while (hashTable[index] != -1) {
        index = (index + 1) % tableSize;
        if (index == originalIndex) return -1; // Table is full
    }
    hashTable[index] = key;
    return index;
}

// Double Hashing
int doubleHashing(vector<int>& hashTable, int key, int tableSize) {
    int hash1 = divisionMethod(key, tableSize);
    int hash2 = 1 + (key % (tableSize - 1)); // Secondary hash function
    int index = hash1;
    int i = 0;
    while (hashTable[index] != -1) {
        i++;
        index = (hash1 + i * hash2) % tableSize;
        if (i == tableSize) return -1; // Table is full
    }
    hashTable[index] = key;
    return index;
}

// Main function to demonstrate
int main() {
    int tableSize = 10;

    // Example of hashing methods
    int key = 12345;
    cout << "Division Method: " << divisionMethod(key, tableSize) << endl;
    cout << "Multiplication Method: " << multiplicationMethod(key, tableSize) << endl;
    cout << "Mid Square Method: " << midSquareMethod(key, tableSize) << endl;
    cout << "Folding Method: " << foldingMethod(key, tableSize) << endl;

    // Example of collision resolution techniques
    vector<vector<int>> chainingTable(tableSize);
    chainingMethod(chainingTable, key, tableSize);
    chainingMethod(chainingTable, 54321, tableSize);

    vector<int> openAddressingTable(tableSize, -1);
    openAddressingLinear(openAddressingTable, key, tableSize);
    openAddressingLinear(openAddressingTable, 54321, tableSize);

    vector<int> doubleHashTable(tableSize, -1);
    doubleHashing(doubleHashTable, key, tableSize);
    doubleHashing(doubleHashTable, 54321, tableSize);

    return 0;
}
