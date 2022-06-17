#include <random>

vector<int> solve(int n) {
    mt19937 rnd(1928);
    vector<int> a(n);
    vector<pair<int, int>> seg, seg2;
    vector<int> lhs, rhs, res, pivots;
    for (int i = 0; i < n; i++) {
        a[i] = i;
    }
    seg.push_back({ 0, n - 1 });
    while (true) {
        lhs.clear();
        rhs.clear();
        seg2.clear();
        pivots.clear();
        for (auto s : seg) {
            int l = s.first;
            int r = s.second;
            if (l >= r) continue;
            int len = (r - l + 1);
            int pivot = a[rnd() % len + l];
            pivots.push_back(pivot);
            for (int i = l; i <= r; i++) {
                lhs.push_back(pivot);
                rhs.push_back(a[i]);
            }
        }
        res.resize(lhs.size());
        ask(lhs, rhs, res);
        int pivot_ind = 0;
        int cur = 0;
        for (auto s : seg) {
            int l = s.first;
            int r = s.second;
            if (l >= r) continue;
            int pivot = pivots[pivot_ind++];
            int le = l;
            for (int i = l; i <= r; ++i) {
                if (res[i - l + cur] == 1) {
                    swap(a[i], a[le]);
                    swap(res[i - l + cur], res[le - l + cur]);
                    le++;
                }
            }
            int gr = r;
            for (int i = r; i >= le; --i) {
                if (res[i - l + cur] == -1) {
                    swap(a[i], a[gr]);
                    swap(res[i - l + cur], res[gr - l + cur]);
                    gr--;
                }
            }
            seg2.push_back({ l, le - 1 });
            seg2.push_back({ gr + 1, r });
            cur += (r - l + 1);
        }
        if (seg2.empty()) break;
        swap(seg2, seg);
    }
    return a;
}