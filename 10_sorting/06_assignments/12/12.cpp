#include <iostream>
#include <string>
#include <vector>

using namespace std;

void fill_vec(vector<int>& a, int n, int start)
{
    a.resize(n);
    a[0] = start;
    for (int i = 1; i < n; ++i) {
        a[i] = (1103515245LL * a[i - 1] + 12345) % (1 << 20);
    }
}

int calc_hash(const vector<int>& a)
{
    int answer = 0;
    for (int i : a) {
        answer = (239LL * answer + i) % 1000000007;
    }
    return answer;
}

void counting_sort(vector<int>& a, const int MAX_VAUE) {
    vector<int> cnt(MAX_VAUE + 1, 0);
    for (const int& x : a) {
        ++cnt[x];
    }
    int ind = 0;
    for (int i = 0; i <= MAX_VAUE; ++i) {
        while (cnt[i] > 0) {
            cnt[i]--;
            a[ind++] = i;
        }
    }
    return;
}

int main() {
    int n, a0;
    vector<int> a;
    cin >> n >> a0;
    fill_vec(a, n, a0);
    counting_sort(a, (1 << 20));
    cout << calc_hash(a);
}