#include <iostream>
#include <vector>

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
    vector<int> a;
    int i;
    while (cin >> i) {
        a.push_back(i);
    }

    qsort(a, 0, a.size() - 1);

    for (int i : a) {
        cout << i << " ";
    }
    cout << endl;

    return 0;
}