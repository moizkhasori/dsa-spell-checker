#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
using namespace std;

// Trie Node
struct TrieNode {
    unordered_map<char, TrieNode*> children;
    bool isEndOfWord = false;
};

// Trie Class
class Trie {
public:
    TrieNode* root;

    Trie() {
        root = new TrieNode();
    }

    // Insert word into the Trie
    void insert(const string& word) {
        TrieNode* node = root;
        for (char ch : word) {
            if (node->children.find(ch) == node->children.end()) {
                node->children[ch] = new TrieNode();
            }
            node = node->children[ch];
        }
        node->isEndOfWord = true;
    }

    // Find all words with a given prefix
    void findWordsWithPrefix(TrieNode* node, const string& prefix, vector<string>& results) {
        if (node->isEndOfWord) {
            results.push_back(prefix);
        }
        for (auto& pair : node->children) {
            findWordsWithPrefix(pair.second, prefix + pair.first, results);
        }
    }

    vector<string> searchByPrefix(const string& prefix) {
        TrieNode* node = root;
        for (char ch : prefix) {
            if (node->children.find(ch) == node->children.end()) {
                return {}; // No words found with this prefix
            }
            node = node->children[ch];
        }

        vector<string> results;
        findWordsWithPrefix(node, prefix, results);
        return results;
    }
};

// Comparator for Max Heap
struct Compare {
    bool operator()(const pair<string, int>& a, const pair<string, int>& b) {
        return a.second < b.second; // Max Heap: Higher frequency has higher priority
    }
};

int main() {
    Trie trie;

    // Generate random words starting with 'a'
    vector<string> words = {
        "apple", "application", "app", "appmera", "arm", "arcade", "arrow", "atom", "area", "atlas",
        "april", "alert", "aside", "awake", "azure", "artist", "arc", "archer", "arena", "apex"
    };

    // Insert words into Trie
    for (const string& word : words) {
        trie.insert(word);
    }

    // Frequency hashmap
    unordered_map<string, int> freqMap;

    // User Input
    string userInput;
    cout << "Enter a prefix (starting with 'a'): ";
    cin >> userInput;

    // Search in Trie
    vector<string> matchedWords = trie.searchByPrefix(userInput);
    if (matchedWords.empty()) {
        cout << "No words found with prefix: " << userInput << endl;
        return 0;
    }

    // Update hashmap with frequencies
    for (const string& word : matchedWords) {
        if (freqMap.find(word) == freqMap.end()) {
            freqMap[word] = 1; // Default frequency
        } else {
            freqMap[word]++; // Increment frequency
        }
    }

    // Push into Max Heap
    priority_queue<pair<string, int>, vector<pair<string, int>>, Compare> maxHeap;
    for (const auto& entry : freqMap) {
        maxHeap.push(entry);
    }

    // Extract from Max Heap and display results
    cout << "\nSuggestions based on frequency:\n";
    while (!maxHeap.empty()) {
        auto top = maxHeap.top();
        cout << top.first << " (Frequency: " << top.second << ")\n";
        maxHeap.pop();
    }


    


















    cout << "Enter a prefix (starting with 'a'): ";
    cin >> userInput;

    // Search in Trie
    matchedWords = trie.searchByPrefix(userInput);
    if (matchedWords.empty()) {
        cout << "No words found with prefix: " << userInput << endl;
        return 0;
    }

    // Update hashmap with frequencies
    for (const string& word : matchedWords) {
        if (freqMap.find(word) == freqMap.end()) {
            freqMap[word] = 1; // Default frequency
        } else {
            freqMap[word]++; // Increment frequency
        }
    }
    
    for (const auto& entry : freqMap) {
        maxHeap.push(entry);
    }

    // Extract from Max Heap and display results
    cout << "\nSuggestions based on frequency:\n";
    while (!maxHeap.empty()) {
        auto top = maxHeap.top();
        cout << top.first << " (Frequency: " << top.second << ")\n";
        maxHeap.pop();
    }



    

    return 0;
}
