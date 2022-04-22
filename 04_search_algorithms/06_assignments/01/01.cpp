#include <iostream>


int formula(int n, int a, int b, int x) {
    double y = ((double)(n - a * x)) / b;
    int y_integer = (int)y;
    return ((y - y_integer) == 0.0) ? y_integer : -1;
}

int binary(int n, int a, int b, int x) {
    int l = 0, r = n;
    while (l <= r) {
        int m = l + (r - l) / 2;
        int tmp = a * x + b * m;
        if (tmp < n) {
            l = m + 1;
        }
        else if (tmp > n) {
            r = m - 1;
        }
        else {
            return m;
        }
    }

    return -1;
}

int main() {

    int n = 0, a = 0, b = 0;
    std::cin >> n >> a >> b;

    int s = 0;
    for (int x = 0; x <= n; ++x) {
//        int y = formula(n, a, b, x);
        int y = binary(n, a, b, x);
        if (y >= 0) {
            s++;
        }
    }
    std::cout << s;

    return 0;
}