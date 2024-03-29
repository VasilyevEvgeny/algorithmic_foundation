#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <sstream>
#include <cassert>

// 1 - work, 2 - manual testing
#define MODE 1

#define VERBOSE false

using size = int64_t;


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


std::ostream& operator << (std::ostream& os, const std::queue<std::vector<size>>& q) {
    auto copy = q;
    os << "\n";
    for (size_t i = 0; i < q.size(); ++i) {
        os << copy.front() << std::endl;
        copy.pop();
    }
    return os;
}


struct Time {
    Time() : hours_(0), minutes_(0), minutes_max_(60) {}
    Time(size hours, size minutes) : hours_(hours), minutes_(minutes), minutes_max_(60) {}

    size hours_;
    size minutes_;
    const size minutes_max_;

    void operator+= (size dt) {

        auto tmp = minutes_ + dt;
        auto minutes = tmp % minutes_max_;
        auto hours = tmp / minutes_max_;

        hours_ += hours;
        minutes_ = minutes;
    }

    Time& operator= (const Time& other) {
        hours_ = other.hours_;
        minutes_ = other.minutes_;

        return *this;
    }
};


std::ostream& operator<< (std::ostream& os, const Time& time) {
    os << time.hours_ << " " << time.minutes_;
    return os;
}


std::ostream& operator << (std::ostream& os, const std::vector<std::pair<Time, size>>& v) {
    os << "\n";
    for (auto& item : v) {
        os << item.first << " " << item.second << std::endl;
    }
    return os;
}


std::ostream& operator << (std::ostream& os, const std::queue<std::pair<Time, size>>& q) {
    auto copy = q;
    os << "\n";
    for (size_t i = 0; i < q.size(); ++i) {
        os << copy.front().first << " " << copy.front().second << std::endl;
        copy.pop();
    }
    return os;
}


std::ostream& operator << (std::ostream& os, const std::queue<std::pair<std::pair<Time, size>, std::string>>& q) {
    auto copy = q;
    os << "\n";
    for (size_t i = 0; i < q.size(); ++i) {
        os << copy.front().first.first << " " << copy.front().first.second << " --> " << copy.front().second << std::endl;
        copy.pop();
    }
    return os;
}


bool operator== (const Time& lhs, const Time& rhs) {
    return lhs.hours_ == rhs.hours_ && lhs.minutes_ == rhs.minutes_;
}


bool operator<= (const Time& lhs, const Time& rhs) {
    if (lhs.hours_ != rhs.hours_) { return lhs.hours_ <= rhs.hours_; }
    else { return lhs.minutes_ <= rhs.minutes_; }
}


bool operator< (const Time& lhs, const Time& rhs) {
    if (lhs.hours_ != rhs.hours_) { return lhs.hours_ < rhs.hours_; }
    else { return lhs.minutes_ < rhs.minutes_; }
}


Time operator+ (const Time& lhs, size dt) {
    Time res(lhs.hours_, lhs.minutes_);
    res += dt;
    return res;
}


class Solution {
public:
    Solution(size N, std::vector<std::pair<Time, size>>& clients)
    : N_(N), clients_(clients), haircut_duration_(20) {
        if (VERBOSE) { std::cout << "CLIENTS: " << clients_ << std::endl; }
    }

    std::string solve() {
        std::string answer;

        for (size i = 0; i < N_; ++i) {
            auto client = clients_[i];
            auto time = client.first;
            auto impatience = client.second;

            if (!q_.empty()) {
                while (q_.front() <= time) {
                    q_.pop();
                    if (q_.empty()) { break; }
                }
            }

            if (q_.empty()) {
                auto haircut_end = time + haircut_duration_;
                q_.push(haircut_end);
                answer += TimeToString(haircut_end) + "\n";
            } else if (q_.size() <= impatience) {
                auto haircut_end = q_.back() + haircut_duration_;
                q_.push(haircut_end);
                answer += TimeToString(haircut_end) + "\n";
            } else {
                answer += TimeToString(time) + "\n";
            }
        }

        return answer;
    }

private:

    static std::string TimeToString(Time& time) {
        return std::to_string(time.hours_) + " " + std::to_string(time.minutes_);
    }

    const size N_;
    const size haircut_duration_;

    std::vector<std::pair<Time, size>> clients_;
    std::queue<Time> q_;
};


class ManualTester {
public:
    ManualTester() = default;

    static void test() {
        std::vector<std::map<std::string, std::string>> test_data = {

            // boundary
            {{"N", "1"}, {"clients", "0 0 0\n"}, {"true", "0 20\n"}},
            {{"N", "1"}, {"clients", "0 58 0\n"}, {"true", "1 18\n"}},
            {{"N", "2"}, {"clients", "0 0 0\n0 1 0\n"}, {"true", "0 20\n0 1\n"}},
            {{"N", "2"}, {"clients", "0 0 0\n0 20 0\n"}, {"true", "0 20\n0 40\n"}},
            {{"N", "2"}, {"clients", "0 0 1\n0 20 0\n"}, {"true", "0 20\n0 40\n"}},
            {{"N", "2"}, {"clients", "0 0 0\n0 21 0\n"}, {"true", "0 20\n0 41\n"}},
            {{"N", "3"}, {"clients", "0 0 0\n0 1 0\n0 2 0\n"}, {"true", "0 20\n0 1\n0 2\n"}},
            {{"N", "3"}, {"clients", "0 0 0\n0 1 1\n0 2 0\n"}, {"true", "0 20\n0 40\n0 2\n"}},
            {{"N", "3"}, {"clients", "0 0 0\n0 1 1\n0 2 1\n"}, {"true", "0 20\n0 40\n0 2\n"}},
            {{"N", "3"}, {"clients", "0 0 0\n0 1 1\n0 2 2\n"}, {"true", "0 20\n0 40\n1 0\n"}},
            {{"N", "3"}, {"clients", "0 0 0\n0 20 0\n0 21 0\n"}, {"true", "0 20\n0 40\n0 21\n"}},
            {{"N", "3"}, {"clients", "0 0 0\n0 20 0\n0 21 1\n"}, {"true", "0 20\n0 40\n1 0\n"}},
            {{"N", "3"}, {"clients", "0 0 0\n0 20 0\n0 39 0\n"}, {"true", "0 20\n0 40\n0 39\n"}},
            {{"N", "3"}, {"clients", "0 0 0\n0 20 0\n0 40 0\n"}, {"true", "0 20\n0 40\n1 0\n"}},

            // basic
            {{"N", "3"}, {"clients", "10 0 0\n10 1 1\n10 2 0\n"}, {"true", "10 20\n10 40\n10 2\n"}},
            {{"N", "5"}, {"clients", "1 0 100\n2 0 0\n2 1 0\n2 2 3\n2 3 0\n"}, {"true", "1 20\n2 20\n2 1\n2 40\n2 3\n"}},
            {{"N", "5"}, {"clients", "0 0 0\n0 1 1\n0 2 2\n0 3 3\n1 20 0\n"}, {"true", "0 20\n0 40\n1 0\n1 20\n1 40\n"}},

            // different
            {{"N", "6"},
             {"clients", "0 0 0\n0 1 1\n0 2 2\n0 3 3\n0 4 4\n0 5 5\n"},
             {"true", "0 20\n0 40\n1 0\n1 20\n1 40\n2 0\n"}},

            {{"N", "6"},
             {"clients", "0 0 0\n0 40 0\n1 20 0\n2 0 0\n2 40 0\n3 40 0\n"},
             {"true", "0 20\n1 0\n1 40\n2 20\n3 0\n4 0\n"}},

            {{"N", "8"},
             {"clients", "0 0 0\n0 19 1\n0 40 0\n0 50 1\n1 0 10\n1 10 2\n1 20 10\n3 0 0\n"},
             {"true", "0 20\n0 40\n1 0\n1 20\n1 40\n2 0\n2 20\n3 20\n"}},

            {{"N", "8"},
             {"clients", "0 0 0\n0 19 1\n0 40 0\n0 50 1\n1 0 1\n1 10 1\n1 11 2\n2 20 0\n"},
             {"true", "0 20\n0 40\n1 0\n1 20\n1 40\n1 10\n2 0\n2 40\n"}},

        };

        for (auto& line : test_data) {
            std::cerr << "===============================" << std::endl;

            size N = static_cast<size>(std::stoi(line.at("N")));
            std::vector<std::pair<Time, size>> clients = parse_str(line.at("clients"));

            std::string res_true = line.at("true");
            std::string pred = Solution(N, clients).solve();

            std::cerr << "N = " << N << "\nclients:" << clients << "pred:\n" << pred << std::endl;

            assert(res_true == pred);
        }
    }

private:

    static std::vector<std::pair<Time, size>> parse_str(const std::string& str) {
        std::stringstream iss(str);
        std::vector<std::pair<Time, size>> res;
        size i = 0, row = -1, a_i = 0, hours = 0, minutes = 0, impatience = 0;
        while (iss >> a_i) {
            if (!(i % 3)) { hours = a_i; row++; }
            else if (i % 3 == 1) { minutes = a_i; }
            else {
                impatience = a_i;
                res.push_back({{hours, minutes}, impatience});
            }
            ++i;
        }

        return res;
    }

};


int main () {

#if MODE == 1
    size N = 0;
    std::cin >> N;

    std::vector<std::pair<Time, size>> clients;
    for (size i = 0; i < N; ++i) {
        size hours = 0, minutes = 0, impatience = 0;
        std::cin >> hours >> minutes >> impatience;
        clients.push_back({{hours, minutes}, impatience});
    }

    std::cout << Solution(N, clients).solve();

#elif MODE == 2
    ManualTester::test();

#endif

    return 0;
}