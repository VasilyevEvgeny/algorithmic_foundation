void fill(vector<int>& a, int n, int start)
{
    a.resize(n);
    a[0] = start;
    for (int i = 1; i < n; ++i) {
        a[i] = (1103515245LL * a[i - 1] + 12345) % (1 << 20);
    }
}