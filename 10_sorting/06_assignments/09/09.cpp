#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

void qsort(vector<int>& a, int l, int r) {
    if (l >= r) {
        return;
    }

    int x = a[(l + r) / 2];
    int i = l, j = r;

    while (i <= j) {
        while (a[i] < x) {
            ++i;
        }
        while (a[j] > x) {
            --j;
        }
        if (i <= j) {
            swap(a[i], a[j]);
            ++i;
            --j;
        }
    }

    qsort(a, l, j);
    qsort(a, i, r);
}

int main() {
//    size_t N = 0;
//    std::cin >> N;
//
//    vector<int> a(N);
//    for (size_t i = 0; i < N; ++i) {
//        std::cin >> a[i];
//    }
//
//    qsort(a, 0, a.size() - 1);
//
//    for (int i : a) {
//        cout << i << " ";
//    }
//    cout << endl;

    size_t n = 2000;
    for (size_t i = 0; i < n; ++i) {
        std::cout << ((i < n / 2) ? i : n - i) << " ";
    }
}