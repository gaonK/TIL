#include <string>
#include <iostream>
using namespace std;

/// time complexity: O(N)
/// space complexity: O(N)
string compress(string str) {

    int originalLength = str.length();
    if (originalLength < 2) {
        return str;
    }

    int count = 1;
    string out = "";
    for (int i = 1; i < originalLength; i++) {
        if (str[i - 1] == str[i]) {
            count++;
        } else {
            out += str[i - 1];
            out += to_string(count);
            count = 1;
        }

        if (out.length() >= originalLength) {
            return str;
        }
    }

    out += str[originalLength - 1];
    out += to_string(count);
    if (out.length() >= originalLength) {
        return str;
    }
    return out;
}