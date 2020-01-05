#include <string>
#include <vector>
#include <bitset>
#include <iostream>
using namespace std;

/// time complexity: O(N) or O(1)
/// space complexity: O(1)
bool isUniqueChars(string str) {
    if (str.size() > 128) {
        return false;
    }

    vector<bool> char_set(128);
    for (int i = 0; i < str.size(); i++) {
        int val = str[i];
        if (char_set[val]) {
            return false;
        }
        char_set[val] = true;
    }
    return true;
}

/// Assume character is between a and z
/// time complexity: O(N) or O(1)
/// space coomplexity: O(1)
bool isUniqueCharsWithBits(string str) {
    int checker = 0;
    for (int i = 0; i < str.size(); i++) {
        int val = str[i];

        if ((checker & (1 << val)) > 0) {
            return false;
        }
        checker |= (1 << val);
    }
    return true;
}

bool isUniqueCharsWithBitVector(string str) {
    bitset<256> bits(0);
    for (int i = 0; i < str.size(); i++) {
        int val = str[i];
        if (bits.test(val) > 0) {
            return false;
        }
        bits.set(val);
    }
    return true;
}

/// time complexity: O(N^2)
/// space complexity: O(1)
bool isUniqueCharsWithNoDataStructure(string str) {
    for (int i = 0; i < str.size() - 1; i++) {
        for (int j = i + 1; j < str.size(); j++) {
            if (str[i] == str[j]) {
                return false;
            }
        }
    }
}