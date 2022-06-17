int calc_hash(const vector<int>& a)
{
    int answer = 0;
    for (int i : a) {
        answer = (239LL * answer + i) % 1000000007;
    }
    return answer;
}