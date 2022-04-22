#include <algorithm>
#include <deque>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class CashBox {
private:
    bool reversed = false;
    deque<int> data;

public:
    void push(int num) {
        if (reversed) {
            data.push_front(num);
        } else {
            data.push_back(num);
        }
    }

    size_t size() {
        return data.size();
    }

    int del() {
        int res;
        if (reversed) {
            res = data.back();
            data.pop_back();
        } else {
            res = data.front();
            data.pop_front();
        }
        return res;
    }

    void reverse() {
        reversed = !reversed;
    }

    void swap(CashBox &other) {
        std::swap(data, other.data);
        std::swap(reversed, other.reversed);
    }

    void move_to(CashBox &other) {
        other.push(del());
    }
};

class Shop {
private:
    CashBox first;
    CashBox second;
    bool reversed = false;
    bool allOpened = true;
    vector<int> served;

    void get_true(int &place) {
        if (reversed) {
            place = (place + 1) % 2;
        }
    }

    void reverse() {
        reversed = !reversed;
    }

public:
    void push(int num, int place) {
        if (!allOpened) {
            second.push(num);
            return;
        }
        get_true(place);
        if (place == 0) {
            first.push(num);
        } else {
            second.push(num);
        }
    }

    void del(int place) {
        if (!allOpened) {
            if (first.size() > 0) {
                served.push_back(first.del() % 10);
            } else {
                served.push_back(second.del() % 10);
            }
            return;
        }
        get_true(place);
        if (place == 0) {
            served.push_back(first.del() % 10);
        } else {
            served.push_back(second.del() % 10);
        }
    }

    void close(int place) {
        get_true(place);
        allOpened = false;
        if (place == 0) {
            first.reverse();
            first.swap(second);
            reverse();
        } else {
            second.reverse();
        }
    }

    void open() {
        size_t commonSize = first.size() + second.size();
        int half = (commonSize + 1) / 2;
        while (first.size() < half) {
            second.move_to(first);
        }
        while (first.size() > half) {
            first.reverse();
            second.reverse();
            first.move_to(second);
            first.reverse();
            second.reverse();
        }
        second.reverse();
        allOpened = true;
    }

    void print() {
        for (auto elem : served) {
            cout << elem;
        }
        cout << '\n';
    }
};

int main() {
    Shop shop;
    int n;
    cin >> n;
    char ty;
    int cur = 1;
    for (int i = 1; i <= n; i++) {
        cin >> ty;
        if (ty == 'a') {
            shop.push(cur++, 0);
        }
        if (ty == 'b') {
            shop.push(cur++, 1);
        }
        if (ty == 'A') {
            shop.del(0);
        }
        if (ty == 'B') {
            shop.del(1);
        }
        if (ty == '>') {
            shop.close(0);
        }
        if (ty == ']') {
            shop.close(1);
        }
        if (ty == '<' || ty == '[') {
            shop.open();
        }
    }
    shop.print();
}