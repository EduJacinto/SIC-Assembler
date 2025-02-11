#include "headers.h"
bool isValidSymbol(string symbol) {
    if (strlen(symbol) > 6) {
        return false;
    }
    for (size_t i = 0; i < strlen(symbol); i++) {
        if (!isalnum(symbol[i])) {
            return false;
        }
    }
    return true;
}