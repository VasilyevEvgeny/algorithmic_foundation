//
// Created by evasilyev on 01.05.2020.
//

#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <chrono>
#include <cassert>

// 1 - work, 2 - manual testing
#define MODE 1

#define VERBOSE false

using tt = std::chrono::steady_clock;

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


class FastSolver {
public:
    explicit FastSolver(std::string s) : s_(std::move(s)) {}

    std::string solve() {
        size_t i = 0;
        for (const auto& bracket : s_) {
            if (brackets_.empty()) {
                if (VERBOSE) { std::cout << "brackets is empty!" << std::endl; }
                if (bracket == ')' or bracket == ']' or bracket == '}') {
                    if (VERBOSE) { std::cout << "inverse bracket " << bracket << ", return NO" << std::endl; }
                    return generate_answer(false);
                }
                else {
                    brackets_.push(bracket);
                    if (VERBOSE) {
                        log_.push_back(bracket);
                        std::cout << "first bracket " << bracket << " added to brackets" << std::endl;
                    }
                }
            }
            else {
                if (bracket == ')' or bracket == ']' or bracket == '}') {
                    if (pairs_.at(brackets_.top()) == bracket) {
                        brackets_.pop();
                        if (VERBOSE) {
                            log_.pop_back();
                            std::cout << "bracket " << bracket << " is paired with top in brackets! brackets --> " << log_ << std::endl;
                        }
                    }
                    else {
                        if (VERBOSE) { std::cout << "wrong paired bracket, return NO" << std::endl; }
                        return generate_answer(false);
                    }
                }
                else {
                    brackets_.push(bracket);
                    if (VERBOSE) {
                        log_.push_back(bracket);
                        std::cout << "bracket " << bracket << " pushed to brackets! brackets --> " << log_ << std::endl;
                    }
                }
            }

            if (VERBOSE) {
                std::cout << "=========================================" << std::endl;
            }


            i++;
        }

        return generate_answer(brackets_.empty());
    }


private:

    static std::string generate_answer(bool res) {
        if (res) { return "YES"; }
        else { return "NO"; }
    }

    const std::string s_;

    std::stack<char> brackets_;
    std::map<char, char> pairs_ = {{'(', ')'}, {'[', ']'}, {'{', '}'}};

    std::vector<char> log_;
};


class ManualTester {
public:
    ManualTester() = default;

    static void test() {
        std::vector<std::map<std::string, std::string>> test_data = {

            // boundary
            {{"s", "("}, {"true", "NO"}},
            {{"s", ")"}, {"true", "NO"}},
            {{"s", "["}, {"true", "NO"}},
            {{"s", "]"}, {"true", "NO"}},
            {{"s", "{"}, {"true", "NO"}},
            {{"s", "}"}, {"true", "NO"}},
            {{"s", "()"}, {"true", "YES"}},
            {{"s", "[]"}, {"true", "YES"}},
            {{"s", "{}"}, {"true", "YES"}},
            {{"s", "([{}])"}, {"true", "YES"}},
            {{"s", "([{(}])"}, {"true", "NO"}},

            // basic
            {{"s", "([]{})[]"}, {"true", "YES"}},
            {{"s", "[(])"},     {"true", "NO"}}
        };

        for (auto &line : test_data) {
            std::string s = line.at("s");
            std::string res_true = line.at("true");

            tt::steady_clock::time_point begin_fast = tt::now();
            auto pred_fast = FastSolver(s).solve();
            tt::steady_clock::time_point end_fast = tt::now();
            auto duration_fast = std::chrono::duration_cast<std::chrono::nanoseconds> (end_fast - begin_fast).count();

            std::cerr << "s = " << s << std::endl;
            std::cerr << std::scientific << "T = " << duration_fast * 1e-9 << " s :: pred_fast = " << pred_fast << std::endl;

            assert(res_true == pred_fast);
        }
    }
};


int main() {

#if MODE == 1
    std::string s;
    std::getline(std::cin, s);

    std::cout << FastSolver(s).solve() << std::endl;

#elif MODE == 2
    ManualTester::test();

#endif

    return 0;
}