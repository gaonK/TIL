#include <string>
#include <iostream>
using namespace std;

bool isRotation(string s1, string s2) {
    int len1 = s1.length();
    int len2 = s2.length();

    if (len1 == 0 || len1 != len2) {
        return false;
    }

    string concatS1 = s1 + s1;
    // isSubstring
    if (concatS1.find(s2) != string::npos) {
        return true;
    } else {
        return false;
    }
}