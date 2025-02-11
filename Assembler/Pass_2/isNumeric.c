#include "headers.h"

bool isNumeric(string str) {
    for (size_t i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i]) && str[i] != '-') {
            return false;
        }
    }
    return true;
}
