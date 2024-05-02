#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstring>
using namespace std;

struct CharPtrHash {
    size_t operator()(const unsigned char* str) const {
        size_t hash = 5381;
        int c;
        while ((c = *str++))
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        return hash;
    }
};

// Custom equality function for const char*
struct CharPtrEqual {
    bool operator()(const unsigned char* a, const unsigned char* b) const {
        return std::strcmp(reinterpret_cast<const char*>(a), reinterpret_cast<const char*>(b)) == 0;
    }
};

vector<string> tokenize(const string& str) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

unordered_map<const unsigned char *, unordered_set<int>, CharPtrHash, CharPtrEqual> build_inverted_index(const vector<string>& files) {
    unordered_map<const unsigned char *, unordered_set<int>, CharPtrHash, CharPtrEqual> invertedIndex;

    const int size = files.size();
    #pragma omp parallel for
    for (int i = 0; i < size; i++) {
        ifstream inFile(files[i]);
        if (!inFile.is_open()) {
            cerr << "Failed to open file: " << files[i] << endl;
            continue;
        }

        string word;
        while (inFile >> word) {
            #pragma omp critical
            {
                const unsigned char * w = reinterpret_cast<const unsigned char*>(word.c_str());
                invertedIndex[w].insert(i + 1);
            }
        }

        inFile.close();
        std::cout << "Archivo " << to_string(i + 1) << " procesado exitosamente\n";
    }

    return invertedIndex;
}

// Function to search the inverted index
void search(const unordered_map<const unsigned char *, unordered_set<int>, CharPtrHash, CharPtrEqual>& invertedIndex, const string& query) {
    const unsigned char * key = reinterpret_cast<const unsigned char*>(query.c_str());

    auto it = invertedIndex.find(key);
    if (it != invertedIndex.end()) {
        cout << "Documentos que contienen \"" << query << "\":" << endl;
        for (const auto& doc : it->second) {
            cout << " - " << doc << endl;
        }
    } else {
        cout << "Ningun documento tiene la palabra \"" << query << "\"" << endl;
    }
}

int main() {
    vector<string> files; // Add your file names here
    for (int i = 0; i < 1; i++)
    {
        files.push_back("zdoc" + to_string(i + 1) + ".txt");
    }
    std::clock_t start = std::clock();
    unordered_map<const unsigned char*, unordered_set<int>, CharPtrHash, CharPtrEqual> invertedIndex;
    invertedIndex = build_inverted_index(files);
    std::clock_t end = std::clock();
    std::cout << "Word count completed in " << (end - start) / (double)CLOCKS_PER_SEC << " seconds\n";
    string query;
    cout << invertedIndex.max_size() << ' ' << invertedIndex.size() << '\n';
    cout << "Enter a word to search for in the documents: ";
    while (cin >> query)
    {
        search(invertedIndex, query);
    }

    return 0;
}