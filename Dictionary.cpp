#include "Dictionary.h"
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <cctype>
#include <utility>
using namespace std;

// MAP
class DictionaryImpl {
  public:
    DictionaryImpl(int maxBuckets) : buckets(maxBuckets) { hashTable.resize(buckets); }
    ~DictionaryImpl() {}
     // O(N)
    void insert(string word) {
        removeNonLetters(word); // O(N)
        if (!word.empty()) {
            unsigned long hashed = hashcode(word);  // O(N)
            hashTable[hashed % buckets].push_back(make_pair(hashed, word));
        }
    }
     // O(N)
    void lookup(string letters, void callback(string)) const {
        if (callback == nullptr) return;
        
        removeNonLetters(letters);  // O(N)
        unsigned long hashed = hashcode(letters);   // O(N)
        if (hashed == -1) return;
        
        const list<pair<unsigned long, string>> wordList = hashTable[hashed % buckets];
        list<pair<unsigned long, string>>::const_iterator it = wordList.begin();
        while (it != wordList.end()) {  // O(N)
            if (it->first == hashed)
                callback(it->second);
            it++;
        }
    }
    
  private:
     // unordered_map
    vector<list<pair<unsigned long, string>>> hashTable;
    int buckets;
    int primes[26];
    
     // generate same hashcode for anagrams
     // O(N)
    unsigned long hashcode(string hash) const {
        removeNonLetters(hash);
        if (hash.empty()) return -1;
        
        unsigned long hashed = 1;
                       // A  B  C  D  E   F   G   H   I   J   K   L   M   N   O   P   Q   R   S   T   U   V   W   X   Y    Z
        int primes[26] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101};
        for (int i = 0; i < hash.size(); ++i)
            hashed *= primes[hash[i] - 'a'];
        return hashed;
    }
     // O(N)
    void removeNonLetters(string& s) const {
        string::iterator to = s.begin();
        for (string::const_iterator from = s.begin(); from != s.end(); from++)
            if (isalpha(*from)) {
                *to = tolower(*from);
                to++;
            }
        s.erase(to, s.end());  // chop everything off from "to" to end.
    }
};
 

Dictionary::Dictionary(int maxBuckets) { m_impl = new DictionaryImpl(maxBuckets); }

Dictionary::~Dictionary() { delete m_impl; }

void Dictionary::insert(string word) { m_impl->insert(word); }

void Dictionary::lookup(string letters, void callback(string)) const { m_impl->lookup(letters, callback); }
