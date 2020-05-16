//
// Created by evasilyev on 13.05.2020.
//

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <list>
#include <random>
#include <chrono>
#include <cassert>

// 1 - work, 3 - stress testing
#define MODE 1

#define VERBOSE false

using size = int64_t;
using tt = std::chrono::steady_clock;


std::ostream& operator << (std::ostream& os, const std::list<std::pair<size, size>>& lst) {
    if (lst.empty()) { os << "nullptr"; return os; }

    os << "nullptr -> ";
    size i = 1;
    for (auto& node : lst) {
        os << "#" << i << ": {" << node.first << ", " << node.second << "} -> ";
        ++i;
    }
    os << "nullptr";

    return os;
}


std::ostream& operator << (std::ostream& os, const std::vector<size>& v) {
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


std::ostream& operator << (std::ostream& os, const std::vector<std::pair<size, std::vector<size>>>& vv) {
    for (auto& item : vv) { os << item.first << ": " << item.second << std::endl; }

    return os;
}


std::ostream& operator << (std::ostream& os, const std::vector<std::list<std::pair<size, size>>>& v) {
    os << "########## GROUPS: ##########\n";
    for (size_t i = 0; i < v.size(); ++i) {
        os << "#" << i << " --> " << v[i] << std::endl;
    }
    return os;
}


class Greedy {
public:
    Greedy(size q, std::vector<std::pair<size, std::vector<size>>>& events)
    : q_(q), events_(events), n_(1e7) { fence_.resize(n_, 0); }

    std::string solve() {
        std::string answer;
        for (auto& item : events_) {
            auto t = item.first;
            if (VERBOSE) { std::cout << "t = " << t << std::endl; }
            size r = 0, c = 0, x = 0;
            if (t == 1) {
                r = item.second[0];
                c = item.second[1];
                if (VERBOSE) { std::cout << "r = " << r << ", c = " << c << std::endl; }

                for (size i = 0; i < r; ++i) { fence_[i] = c; }
            }
            else if (t == 2) {
                x = item.second[0];
                if (VERBOSE) { std::cout << "x = " << x << std::endl; }
                answer += std::to_string(fence_[x - 1]) + "\n";
            }
        }

        return answer;
    }

private:
    size q_;
    std::vector<std::pair<size, std::vector<size>>> events_;

    size n_;
    std::vector<size> fence_;
};


class Fast {
public:
    Fast(size q, std::vector<std::pair<size, std::vector<size>>>& events)
    : q_(q), events_(events), sqrt_val_(317), max_group_size_(2 * sqrt_val_) {
        groups_.emplace_back(std::list<std::pair<size, size>>());

        if (VERBOSE) { std::cout << groups_ << std::endl; }
    }

    std::string solve() {
        std::string answer;

        for (auto& item : events_) {
            if (VERBOSE) { std::cout << "==========================" << std::endl; }

            auto t = item.first;
            if (VERBOSE) { std::cout << "t = " << t << std::endl; }
            size r = 0, c = 0, x = 0;

            if (t == 1) {
                r = item.second[0];
                c = item.second[1];
                if (VERBOSE) { std::cout << "r = " << r << ", c = " << c << std::endl; }

                if (groups_.size() == 1 && groups_[0].empty()) { groups_[0].emplace_back(r, c); }
                else {
                    bool less_than_others = true;
                    size n_target = 0;
                    for (size n = 0; n < groups_.size(); ++n) {
                        auto cur_high = groups_[n].front().first;
                        auto cur_low = groups_[n].back().first;
                        if (r >= cur_low) {
                            for (auto it = groups_[n].begin(); it != groups_[n].end(); ++it) {
                                if (r >= (*it).first) {
                                    // insert
                                    groups_[n].insert(it, {r, c});
                                    if (VERBOSE) { std::cout << "{" << r << ", " << c << "} inserted before {" <<
                                                   (*it).first << ", " << (*it).second << "}" << std::endl; }

                                    // erase remainder of cur group and other groups up to end
                                    groups_[n].erase(it, groups_[n].end());
                                    groups_.resize(n + 1);
                                    less_than_others = false;

                                    n_target = n;
                                    break;
                                }
                            }

                            break;
                        }
                    }
                    if (less_than_others) {
                        auto last_index = groups_.size() - 1;
                        groups_[last_index].emplace_back(r, c);
                        n_target = last_index;
                        if (VERBOSE) { std::cout << "less than others! pushed back" << std::endl; }
                    }

                    if (groups_[n_target].size() == max_group_size_) {
                        // insert new group
                        auto it = groups_.begin() + n_target + 1;
                        groups_.insert(it, std::list<std::pair<size, size>>());

                        // copy remainder to new group and del remainder
                        auto copy_it = groups_[n_target].begin();
                        auto del_it = groups_[n_target].begin();
                        size half = max_group_size_ / 2;
                        if (VERBOSE) { std::cout << "half = " << half << std::endl; }
                        for (size i = 0; i < groups_[n_target].size(); ++i) {
                            if (i < half) { del_it = std::next(del_it); }
                            else { groups_[n_target + 1].push_back(*copy_it); }
                            copy_it = std::next(copy_it);
                        }
                        groups_[n_target].erase(del_it, groups_[n_target].end());
                    }
                }
            }
            else {
                x = item.second[0];
                if (VERBOSE) { std::cout << "x = " << x << std::endl; }

                bool greater_than_others = true;
                for (auto it_group = groups_.rbegin(); it_group != groups_.rend(); ++it_group) {
                    auto cur_high = (*it_group).front().first;
                    auto cur_low = (*it_group).back().first;
                    if (VERBOSE) { std::cout << "x = " << x << ", cur_low = " << cur_low << std::endl; }

                    if (x <= cur_high) {
                        for (auto it = (*it_group).rbegin(); it != (*it_group).rend(); ++it) {
                            if (VERBOSE) { std::cout << "x = " << x << ", (*it).first = " << (*it).first << std::endl; }
                            if (x <= (*it).first) {
                                std::string cur_answer = std::to_string((*it).second);
                                answer += cur_answer + "\n";
                                greater_than_others = false;

                                if (VERBOSE) { std::cout << "cur_answer = " << cur_answer << std::endl; }

                                break;
                            }
                        }
                        break;
                    }
                }
                if (greater_than_others) {
                    answer += "0\n";
                    if (VERBOSE) { std::cout << "greater than others! cur_answer = 0" << std::endl; }
                }
            }

            if (VERBOSE) { std::cout << groups_ << std::endl; }
        }

        return answer;
    }

//    std::string solve() {
//        std::string answer;
//
//        for (auto& item : events_) {
//            if (VERBOSE) { std::cout << "==========================" << std::endl; }
//
//            auto t = item.first;
//            if (VERBOSE) { std::cout << "t = " << t << std::endl; }
//            size r = 0, c = 0, x = 0;
//            if (t == 1) {
//                r = item.second[0];
//                c = item.second[1];
//                if (VERBOSE) { std::cout << "r = " << r << ", c = " << c << std::endl; }
//
//                if (fence_stats_.empty()) {
//                    fence_stats_.emplace_back(r, c);
//                    if (VERBOSE) { std::cout << "empty! added first element" << std::endl; }
//                }
//                else {
//                    bool less_than_others = true;
//                    for (auto it = fence_stats_.begin(); it != fence_stats_.end(); ++it) {
//                        if (r >= (*it).first) {
//                            fence_stats_.insert(it, {r, c});
//                            if (VERBOSE) { std::cout << "{" << r << ", " << c << "} inserted before {" <<
//                                           (*std::next(it)).first << ", " << (*std::next(it)).second << "}" << std::endl; }
//
//                            fence_stats_.erase(it, fence_stats_.end());
//                            less_than_others = false;
//
//                            break;
//                        }
//                    }
//                    if (less_than_others) {
//                        fence_stats_.emplace_back(r, c);
//                        if (VERBOSE) { std::cout << "less than others! pushed back" << std::endl; }
//                    }
//                }
//
//                if (VERBOSE) { std::cout << "fence_stats_: " << fence_stats_ << std::endl; }
//            }
//            else if (t == 2) {
//                x = item.second[0];
//                if (VERBOSE) { std::cout << "x = " << x << std::endl; }
//
//                bool greater_than_others = true;
//                for (auto it = fence_stats_.rbegin(); it != fence_stats_.rend(); ++it) {
//                    if (x <= (*it).first) {
//                        std::string cur_answer = std::to_string((*it).second);
//                        answer += cur_answer + "\n";
//                        greater_than_others = false;
//
//                        if (VERBOSE) { std::cout << "cur_answer = " << cur_answer << std::endl; }
//
//                        break;
//                    }
//                }
//                if (greater_than_others) {
//                    answer += "0\n";
//                    if (VERBOSE) { std::cout << "greater than others! cur_answer = 0" << std::endl; }
//                }
//            }
//        }
//
//        return answer;
//    }

private:
    size q_;
    std::vector<std::pair<size, std::vector<size>>> events_;

    const size sqrt_val_;
    const size max_group_size_;
    std::vector<std::list<std::pair<size, size>>> groups_;

};


//class ManualTester {
//public:
//    ManualTester() = default;
//
//    static void test() {
//        std::vector<std::map<std::string, std::string>> test_data = {
//
//                // basic
//                {{"n", "1"}, {"s", "a"}, {"m", "1"}, {"t", "a"}, {"true", "1"}},
//
//
//        };
//
//        for (auto& line : test_data) {
//            size n = static_cast<size>(std::stoi(line.at("n")));
//            std::string s = line.at("s");
//            size m = static_cast<size>(std::stoi(line.at("m")));
//            std::string t = line.at("t");
//
//            size res_true = static_cast<size>(std::stoi(line.at("true")));
//            auto pred_greedy = GreedySolver(n, s, m, t).solve();
//            auto pred_fast = FastSolver(n, s, m, t).solve();
//
//            std::cerr << "n = " << n << ", s = " << s << ", m = " << m <<  ", t = " << t << ", pred_greedy = " <<
//                      pred_greedy << ", pred_fast = " << pred_fast << std::endl;
//
//            assert(res_true == pred_greedy);
//            assert(res_true == pred_fast);
//        }
//
//    }
//
//private:
//
//
//};


class StressTester {
public:
    StressTester() = default;

    static void test() {
        std::random_device rd;
        std::mt19937 gen(rd());

        const size MIN_Q = 1, MAX_Q = 100;
        std::uniform_int_distribution<> dist_Q(MIN_Q, MAX_Q);

        const size MIN_T = 1, MAX_T = 2;
        std::uniform_int_distribution<> dist_T(MIN_T, MAX_T);

        const size MIN_R = 1, MAX_R = 100;
        std::uniform_int_distribution<> dist_R(MIN_R, MAX_R);

        const size MIN_C = 1, MAX_C = 100;
        std::uniform_int_distribution<> dist_C(MIN_C, MAX_C);

        const size MIN_X = 1, MAX_X = 100;
        std::uniform_int_distribution<> dist_X(MIN_X, MAX_X);

        while (true) {

            std::cout << "=======================================" << std::endl;

            size q = dist_Q(gen);
            std::vector<std::pair<size, std::vector<size>>> events;
            for (size i = 0; i < q; ++i) {
                size t = dist_T(gen);
                if (t == 1) {
                    size r = dist_R(gen);
                    size c = dist_C(gen);
                    events.push_back({t, {r, c}});
                }
                else {
                    size x = dist_X(gen);
                    events.push_back({t, {x}});
                }
            }

            std::cout << "q = " << q << std::endl;
            std::cout << "Events:\n" << events << std::endl;

            tt::steady_clock::time_point begin_greedy = tt::now();
            auto res_greedy = Greedy(q, events).solve();
            tt::steady_clock::time_point end_greedy = tt::now();
            auto duration_greedy = std::chrono::duration_cast<std::chrono::nanoseconds> (end_greedy - begin_greedy).count();

            std::cout << res_greedy << std::endl;
            std::cout << std::scientific << "T_greedy = " << duration_greedy * 1e-9 << " s" << std::endl;

            std::cout << "-----------------------" << std::endl;

            tt::steady_clock::time_point begin_fast = tt::now();
            auto res_fast = Fast(q, events).solve();
            tt::steady_clock::time_point end_fast = tt::now();
            auto duration_fast = std::chrono::duration_cast<std::chrono::nanoseconds> (end_fast - begin_fast).count();

            std::cout << res_fast << std::endl;
            std::cout << std::scientific << "T_fast = " << duration_fast * 1e-9 << " s" << std::endl;

            assert(res_greedy == res_fast);
        }
    }

private:

};


int main() {

#if MODE == 1
    size q = 0;
    std::cin >> q;
    std::cin.ignore();

    std::vector<std::pair<size, std::vector<size>>> events;
    for (size i = 0; i < q; ++i) {
        std::string line;
        std::getline(std::cin, line);
        std::istringstream iss(line);

        size t = 0;
        iss >> t;

        std::vector<size> v;
        size par1 = 0, par2 = 0;
        iss >> par1 >> par2;
        v.push_back(par1);
        if (par2) { v.push_back(par2); }
        events.emplace_back(t, v);
    }

    if (VERBOSE) { std::cout << events << std::endl; }

//    std::cout << Greedy(q, events).solve();
    std::cout << Fast(q, events).solve();

#elif MODE == 3
    StressTester::test();

#endif

    return 0;
}