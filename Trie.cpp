#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <set>
#include <string>
#include <algorithm>
using namespace std;

// TrieNode class definition
class TrieNode {
public:
    char data;
    TrieNode* children[26];
    bool isTerminal;

    TrieNode(char ch) {
        data = ch;
        for (int i = 0; i < 26; i++) {
            children[i] = NULL;
        }
        isTerminal = false;
    }
};

// Trie class definition
class Trie {
public:
    TrieNode* root;

    Trie() {
        root = new TrieNode('\0');
    }

    void insert(string word) {
        TrieNode* node = root;

        for (char ch : word) {
            int index = ch - 'a';
            if (node->children[index] == NULL) {
                node->children[index] = new TrieNode(ch);
            }
            node = node->children[index];
        }
        node->isTerminal = true;
    }

    vector<string> findWordsWithPrefix(string prefix) {
        TrieNode* node = root;

        for (char ch : prefix) {
            int index = ch - 'a';
            if (node->children[index] == NULL) {
                return {};
            }
            node = node->children[index];
        }

        vector<string> results;
        collectWords(node, prefix, results);
        return results;
    }

private:
    void collectWords(TrieNode* node, string prefix, vector<string>& results) {
        if (node->isTerminal) {
            results.push_back(prefix);
        }

        for (int i = 0; i < 26; i++) {
            if (node->children[i] != NULL) {
                char childChar = 'a' + i;
                collectWords(node->children[i], prefix + childChar, results);
            }
        }
    }
};

// Comparator for Max Heap
struct Compare {
    bool operator()(pair<string, int>& a, pair<string, int>& b) {
        return a.second < b.second;
    }
};

// Function to generate possible combinations of a prefix
vector<string> generateCombinations(string prefix, int startIndex) {
    vector<string> combinations;

    // Swap adjacent characters starting from the failing index
    for (int i = startIndex; i < prefix.size() - 1; i++) {
        string swapped = prefix;
        swap(swapped[i], swapped[i + 1]);
        combinations.push_back(swapped);
    }

    // Replace each character with 'a' to 'z' starting from the failing index
    for (int i = startIndex; i < prefix.size(); i++) {
        for (char ch = 'a'; ch <= 'z'; ch++) {
            if (prefix[i] != ch) {
                string replaced = prefix;
                replaced[i] = ch;
                combinations.push_back(replaced);
            }
        }
    }

    return combinations;
}

vector<pair<string, int>> handleMisspelledPrefix(
    Trie& trie, unordered_map<string, int>& wordFrequency, string prefix) {
    vector<vector<string>> arrays; // Store individual arrays

    TrieNode* node = trie.root;
    string validPrefix = "";

    // Step 1: Find the last valid prefix in the Trie
    for (char ch : prefix) {
        int index = ch - 'a';
        if (node->children[index] != NULL) {
            validPrefix += ch;
            node = node->children[index];
        } else {
            break;
        }
    }

    // If no valid prefix exists, return an empty array
    if (validPrefix.empty()) {
        return {};
    }

    // Step 2: Check if validPrefix alone has suggestions
    vector<string> validPrefixResults = trie.findWordsWithPrefix(validPrefix);
    if (!validPrefixResults.empty()) {
        arrays.push_back(validPrefixResults);
    }

    // Step 3: Generate combinations from the failing point
    int failingIndex = validPrefix.size();
    vector<string> combinations = generateCombinations(prefix, failingIndex);

    // Step 4: Collect suggestions for each combination
    for (string combo : combinations) {
        vector<string> results = trie.findWordsWithPrefix(combo);
        if (!results.empty()) {
            arrays.push_back(results);
        }
    }

    // Step 5: Merge arrays and eliminate duplicates
    set<string> mergedSet;
    for (const auto& array : arrays) {
        for (const string& word : array) {
            mergedSet.insert(word);
        }
    }

    // Step 6: Sort merged set by frequency
    priority_queue<pair<string, int>, vector<pair<string, int>>, Compare> maxHeap;
    for (const string& word : mergedSet) {
        maxHeap.push({word, wordFrequency[word]});
    }

    vector<pair<string, int>> sortedSuggestions;
    while (!maxHeap.empty()) {
        sortedSuggestions.push_back(maxHeap.top());
        maxHeap.pop();
    }

    return sortedSuggestions;
}

vector<pair<string, int>> getSuggestions(
    Trie& trie, unordered_map<string, int>& wordFrequency, string prefix) {
    vector<string> suggestions = trie.findWordsWithPrefix(prefix);

    if (suggestions.empty()) {
        cout << "No exact match found. Trying fallback logic...\n";
        return handleMisspelledPrefix(trie, wordFrequency, prefix);
    }

    for (string word : suggestions) {
        wordFrequency[word]++;
    }

    priority_queue<pair<string, int>, vector<pair<string, int>>, Compare> maxHeap;
    for (string word : suggestions) {
        maxHeap.push({word, wordFrequency[word]});
    }

    vector<pair<string, int>> sortedSuggestions;
    while (!maxHeap.empty()) {
        sortedSuggestions.push_back(maxHeap.top());
        maxHeap.pop();
    }

    return sortedSuggestions;
}

int main() {
    Trie trie;
    unordered_map<string, int> wordFrequency;

    vector<string> initialWords = {
        "apple", "app", "april", "bat", "ball", "batman", "banana", 
        "cat", "call", "camel"
    };
    for (string word : initialWords) {
        trie.insert(word);
    }

    while (true) {
        string prefix;
        cout << "\nEnter a prefix to search (or 'exit' to quit): ";
        cin >> prefix;

        if (prefix == "exit") break;

        vector<pair<string, int>> suggestions = getSuggestions(trie, wordFrequency, prefix);

        if (suggestions.empty()) {
            cout << "No suggestions found for \"" << prefix << "\".\n";
        } else {
            cout << "Suggestions for \"" << prefix << "\":\n";
            for (auto& suggestion : suggestions) {
                cout << "- " << suggestion.first << " (Frequency: " << suggestion.second << ")\n";
            }
        }
    }

    return 0;
}
