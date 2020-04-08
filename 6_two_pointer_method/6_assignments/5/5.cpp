//
// Created by evasilyev on 01.04.2020.
//

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <cassert>

#define VERBOSE false

// 1 - work, 2 - manual testing
#define MODE 1

using size = int64_t;


template <typename T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& v) {
    os << "[ ";
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i != v.size() - 1) {
            os << " ";
        }
    }
    os << " ]";
    return os;
}


class GreedySolver {
public:
    GreedySolver(size N, std::vector<size>& a, size M)
    : N_(N), a_(a), M_(M) {}

    size solve() {

        // volume_max
        size a_max = *std::max_element(a_.begin(), a_.end());
        size a_min = *std::min_element(a_.begin(), a_.end());
        size volume_min = 0, volume_max = 0;
        if (N_ <= M_) { return a_max; }
        else if (N_ > 2 *  M_) { return -1; }
        else { volume_min = a_max; volume_max = 2 * a_max; }
        if (VERBOSE) { std::cout << "volume_max = " << volume_max << std::endl; }

        // main cycle
        for (size volume = volume_min; volume < volume_max; ++volume) {
            bool placed = false;

            std::vector<size> boxes(M_, volume);
            for (auto n = 0; n < N_; ++n) {
                if (!a_[n]) { continue; }
                placed = false;
                for (auto m = 0; m < M_; ++m) {
                    if (boxes[m] >= a_[n]) {
                        boxes[m] -= a_[n];
                        placed = true;
                        break;
                    }
                }
                if (!placed) { break; }
            }

            if (placed) { return volume; }
        }

        return -1;
    }


private:
    const size N_;
    const std::vector<size> a_;
    const size M_;

    
};



std::vector<size> str_to_vector(const std::string& str) {
    std::stringstream iss(str);
    std::vector<size> res;
    size a_i = 0;
    while (iss >> a_i)
        res.push_back(a_i);

    return res;
}

void manual_testing() {
    std::vector<std::map<std::string, std::string>> test_data = {

        // basic
        {{"N", "1"}, {"a", "1"}, {"M", "1"}, {"true", "1"}},
        {{"N", "1"}, {"a", "2"}, {"M", "1"}, {"true", "2"}},
        {{"N", "1"}, {"a", "3"}, {"M", "1"}, {"true", "3"}},
        {{"N", "2"}, {"a", "1 2"}, {"M", "1"}, {"true", "3"}},
        {{"N", "3"}, {"a", "1 2 3"}, {"M", "1"}, {"true", "-1"}},

        // medium
        {{"N", "4"}, {"a", "3 1 2 5"}, {"M", "1"}, {"true", "-1"}},
        {{"N", "4"}, {"a", "3 1 2 5"}, {"M", "2"}, {"true", "6"}},
        {{"N", "4"}, {"a", "3 1 2 5"}, {"M", "3"}, {"true", "5"}},
        {{"N", "4"}, {"a", "3 1 2 5"}, {"M", "4"}, {"true", "5"}},
        {{"N", "4"}, {"a", "3 1 2 5"}, {"M", "5"}, {"true", "5"}},


    };

    for (auto& line : test_data) {
        size N = static_cast<size>(std::stoi(line.at("N")));
        std::vector<size> a = str_to_vector(line.at("a"));
        size M = static_cast<size>(std::stoi(line.at("M")));

        size res_true = static_cast<size>(std::stoi(line.at("true")));
        auto pred_greedy = GreedySolver(N, a, M).solve();

        std::cerr << "N = " << N << ", a = " << a << ", M = " << M << ", pred_greedy = " <<
                  pred_greedy << std::endl;

        assert(res_true == pred_greedy);
//        assert(res_true == pred_fast);
    }
}




int main() {

#if MODE == 1
    size N = 0, M = 0;
    std::cin >> N >> M;

    std::vector<size> a(N);
    for (auto i = 0; i < N; ++i) {
        std::cin >> a[i];
    }

    if (VERBOSE) {
        std::cout << "N = " << N << std::endl;
        std::cout << "a = " << a << std::endl;
        std::cout << "M = " << M << std::endl;
    }

    std::cout << GreedySolver(N, a, M).solve() << std::endl;

#elif MODE == 2
    manual_testing();

#elif MODE == 3


#endif

    return 0;
}