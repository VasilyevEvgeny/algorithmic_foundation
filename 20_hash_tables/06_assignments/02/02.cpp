#include <iostream>
#include <map>
#include <unordered_set>
#include <string>
#include <utility>


int main() {

    size_t n = 0;
    std::cin >> n;

    std::map<std::string, std::pair<std::unordered_set<int64_t>, int64_t>> data;
    for (size_t i = 0; i < n; ++i) {
        std::string name;
        std::cin >> name;
        size_t t = 0;
        std::cin >> t;
        if (t == 1) {
            int64_t x;
            std::cin >> x;
            if (data.find(name) != data.end()) {
                if (data[name].first.find(x) == data[name].first.end()) {
                    data[name].first.insert(x);
                    data[name].second += x;
                }
            } else {
                data.insert({name, std::make_pair(std::unordered_set<int64_t>({x}), x)});
            }
        } else if (t == 2) {
            int64_t x;
            std::cin >> x;
            if (data.find(name) != data.end()) {
                if (data[name].first.find(x) != data[name].first.end()) {
                    data[name].first.erase(x);
                    data[name].second -= x;
                }
            }
        } else {
            std::cout << data[name].second << std::endl;
        }

    }

    return 0;
}