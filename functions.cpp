//
// Created by evasilyev on 03.02.2020.
//

#include "functions.h"

std::ostream& operator<<(std::ostream& os, const std::vector<bool>& v)
{
    os << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i]) {
            os << 1;
        }
        else {
            os << 0;
        }

        if (i != v.size() - 1)
            os << ", ";
    }
    os << "]\n";
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<int>& v)
{
    os << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i != v.size() - 1)
            os << ", ";
    }
    os << "]\n";
    return os;
}