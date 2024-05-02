#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class InvertedIndex {
public:
    void addDocument(int docID, const string& text) {
        vector<string> terms = split(text);
        for (const auto& term : terms) {
            index[term].insert(docID);
        }
    }

    vector<int> search(const string& query) {
        vector<string> terms = split(query);
        unordered_set<int> result;
        if (index.find(terms[0]) != index.end()) {
            result = index[terms[0]];
        }

        for (size_t i = 1; i < terms.size(); ++i) {
            const string& term = terms[i];
            if (index.find(term) != index.end()) {
                auto& docIDs = index[term];
                unordered_set<int> newResult;
                for (int docID : docIDs) {
                    if (result.count(docID) > 0) {
                        newResult.insert(docID);
                    }
                }
                result = newResult;
            } else {
                result.clear(); // No document contains this term, so clear result
                break;
            }
        }

        return vector<int>(result.begin(), result.end());
    }

private:
    unordered_map<string, unordered_set<int>> index;

    vector<string> split(const string& text) {
        vector<string> terms;
        string term;
        for (char c : text) {
            if (isspace(c)) {
                if (!term.empty()) {
                    terms.push_back(term);
                    term.clear();
                }
            } else {
                term += c;
            }
        }
        if (!term.empty()) {
            terms.push_back(term);
        }
        return terms;
    }
};

int main() {
    InvertedIndex index;
    
    // Adding documents to the index
    index.addDocument(1, "apple banana cherry");
    index.addDocument(2, "banana date");
    index.addDocument(3, "cherry fig grape");
    
    // Searching for documents containing "banana cherry"
    vector<int> result = index.search("banana cherry");
    cout << "Documents containing 'banana' and 'cherry': ";
    for (int docID : result) {
        cout << docID << " ";
    }
    cout << endl;

    return 0;
}