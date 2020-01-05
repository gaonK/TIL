#include <string>
#include <vector>
#include <iostream>
using namespace std;

/// Helper methods
int getCharIndex(char c) {
    int index = -1;

    if ('a' <= c && c <= 'z') {
        index = c - 'a';
    } else if ('A' <= c && c <= 'Z') {
        index = c - 'A';
    }

    return index;
}

void countFrequency(string str, vector<int> &frequency) {
    int index;
    for (char c : str) {
        index = getCharIndex(c);
        if (index != -1) {
            frequency[index]++;
        }
    }
}

/// time complexity: O(N)
/// space complexity: O(1)
/// At first, count the number of characters
/// If there is only an odd char, it's ok
/// But if there are more than one odd chars, return false
bool isPermutationOfPaalindrome1(string str) {
    vector<int> frequency(26);
    countFrequency(str, frequency);

    bool oddAppeared = false;
    
    for (int i = 0; i < 26; i++) {
        if (frequency[i] % 2 && oddAppeared) {
            return false;
        } else if (frequency[i] % 2 && !oddAppeared) {
            oddAppeared = true;
        }
    }
    return true;
}

/// time complexity: O(N)
/// space complexity: O(1)
/// During counting, check the number of odd chars
/// If there are more than one odd chars, return false
bool isPermutationOfPalindrome2(string str) {
    int oddCount = 0;
    vector<int> frequency(26);
    int index = 0;

    for (char c : str) {
        index = getCharIndex(c);

        if (index != -1) {
            frequency[index]++;
            if (frequency[index] % 2) {
                oddCount++;
            } else {
                oddCount--;
            }
        }
    }

    return (oddCount <= 1);
}

/// Helper methods
int toggle(int bitVector, int index) {
    if (index < 0) {
        return bitVector;
    }

    return bitVector ^ (1 << index);    
}

bool isExactlyOneBitSet(int bitVector) {
    return ((bitVector) & (bitVector - 1) == 0);
}

/// time complexity: O(N)
/// space complexity: O(N)
/// Set bits with xor operation
/// If there are more than one set bits return false
bool isPermutationOfPalindrome3(string str) {
    int bitVector = 0;
    int index = 0;

    for (char c : str) {
        index = getCharIndex(c);
        bitVector = toggle(bitVector, index);
    }

    return (bitVector == 0 || isExactlyOneBitSet(bitVector));
}