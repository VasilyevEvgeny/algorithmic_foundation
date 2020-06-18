#include <iostream>
#include <vector>

class MaxHeap {
private:
    std::vector<int> data;
public:
    explicit MaxHeap(std::vector<int>& a) {
        for (int value : a) {
            insert(value);
        }
    }

    static bool cmp(int x, int y) {
        return x > y;
    }

    int size() const {
        return static_cast<int>(data.size());
    }

    void insert(int value) {
        data.push_back(value);
        sift_up(size() - 1);
    }

    int extract() {
        int x = data[0];
        std::swap(data[0], data.back());
        data.pop_back();
        sift_down(0);
        return x;
    }

    void sift_up(int i) {
        int parent = (i - 1) / 2;
        if (cmp(data[i], data[parent])) {
            std::swap(data[i], data[parent]);
            sift_up(parent);
        }
    }

    void sift_down(int i) {
        int left = 2 * i + 1;
        int right = left + 1;
        if (left < size()) {
            int next = left;
            if (right < size()
                && cmp(data[right], data[left])) {
                next = right;
            }
            if (cmp(data[next], data[i])) {
                std::swap(data[next], data[i]);
                sift_down(next);
            }
        }
    }
};

class Solution {
public:
    int lastStoneWeight(std::vector<int>& stones) {
        MaxHeap heap(stones);

        while (heap.size() > 1) {
            int x = heap.extract();
            int y = heap.extract();
            heap.insert(x - y);
        }
        // heap.size() == 1
        return heap.extract();
    }
};


int main() {


    return 0;
}