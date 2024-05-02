#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>

using namespace std;

vector<string> tokenize(const string& str) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

unordered_map<string, unordered_set<int>> build_inverted_index(const vector<string>& files) {
    unordered_map<string, unordered_set<int>> invertedIndex;

    const int size = files.size();

    for (int i = 0; i < size; i++) {
        ifstream inFile(files[i]);
        if (!inFile.is_open()) {
            cerr << "Failed to open file: " << files[i] << endl;
            continue;
        }

        string word;
        while (inFile >> word) {
            invertedIndex[word].insert(i + 1);
        }

        inFile.close();
        std::cout << "Archivo " << to_string(i + 1) << " procesado exitosamente\n";
    }

    return invertedIndex;
}

// Function to search the inverted index
void search(const unordered_map<string, unordered_set<int>>& invertedIndex, const string& query) {
    auto it = invertedIndex.find(query);
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
    for (int i = 0; i < 10; i++)
    {
        files.push_back("zdoc" + to_string(i + 1) + ".txt");
    }
    std::clock_t start = std::clock();
    unordered_map<string, unordered_set<int>> invertedIndex = build_inverted_index(files);
    std::clock_t end = std::clock();
    std::cout << "Word count completed in " << (end - start) / (double)CLOCKS_PER_SEC << " seconds\n";
    string query;
    cout << "Enter a word to search for in the documents: ";
    while (cin >> query)
    {
        search(invertedIndex, query);
    }

    return 0;
}