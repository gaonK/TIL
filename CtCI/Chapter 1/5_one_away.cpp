#include <string>
#include <cmath>
#include <iostream>
using namespace std;

/// time complexity: O(N)
/// space complexity: O(N)
bool oneEditAway(string str1, string str2) {
    if (abs(int(str1.size()) - int(str2.size()))  > 1) {
        return false;
    }

    int len1 = str1.size();
    int len2 = str2.size();

    string smaller = len1 < len2 ? str1 : str2;
    string bigger = len1 < len2 ? str2 : str1;

    int i = 0, j = 0;
    bool mismatchDone = false;
    while (i < smaller.size() && j < bigger.size()) {
        if (smaller[i] != bigger[j]) {
            if (mismatchDone) {
                return false;
            }
            mismatchDone = true;
            if (len1 == len2) {
                i++;
            }
        } else {
            i++;
        }
        j++;
    }
}