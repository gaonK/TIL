#include <cstring>

/// time complexity: O(N)
/// space complexity: O(1)
void urlify(char *str, int len) {
    int spaceCount = 0;
    
    for (int i = 0; i < len; i++) {
        if (str[i] == ' ') {
            spaceCount++;
        }
    }

    int extendedLen = len + 2 * spaceCount;
    int index = extendedLen - 1;
    for (int i = len - 1; i >= 0; i--) {
        if (str[i] != ' ') {
            str[index--] = str[i];
        } else {
            str[index--] = '0';
            str[index--] = '2';
            str[index--] = '%';
        }
    }
}