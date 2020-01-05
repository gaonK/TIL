#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

/// time complexity: O(NlogN)
/// space complexity: O(1)
bool permutation(string s, string t) {
    if (s.size() != t.size()) {
        return false;
    }

    sort(s.begin(), s.end());
    sort(t.begin(), t.end());

    return s == t;
}

/// Assume character set is ASCII
/// time complexity: O(N)
/// space complexity: O(1)
bool permutationWithCount(string s, string t) {
    if (s.size() != t.size()) { 
        return false;
    }

    vector<int> letters(128);
    for (char c : s) {
        letters[c]++;
    }

    for (int i = 0; i < t.length(); i++) {
        int c = t[i];
        letters[c]--;
        if (letters[c] < 0) {
            return false;
        }
    }

    return true;
}
