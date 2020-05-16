//
// Created by evasilyev on 16.05.2020.
//

#include <iostream>
#include <queue>

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


bool operator<= (const Time& lhs, const Time& rhs) {
    if (lhs.hours_ != rhs.hours_) { return lhs.hours_ <= rhs.hours_; }
    else { return lhs.minutes_ <= rhs.minutes_; }
}

Time operator+ (const Time& lhs, size dt) {
    Time res(lhs.hours_, lhs.minutes_);
    res += dt;
    return res;
}


std::ostream& operator<< (std::ostream& os, Time& time) {
    os << time.hours_ << " " << time.minutes_;
    return os;
}


class Solution {
public:
    Solution(size N, std::queue<std::vector<size>>& clients)
    : N_(N), clients_(clients), haircut_duration_(20) {

        if (VERBOSE) { std::cout << "CLIENTS: " << clients_ << std::endl; }

    }

    void solve() {
        // clients
        while (!clients_.empty()) {
            if (VERBOSE) { std::cout << "===================================" << std::endl; }

            auto client = clients_.front();
            Time client_time(client[0], client[1]);
            auto client_impatience = client[2];

            if (VERBOSE) {
                std::cout << "client time: " << client_time << ", impatience: " << client_impatience << std::endl;
            }

            if (cur_client_.empty()) {
                cur_client_ = client;
                haircut_end_ = client_time + haircut_duration_;

                std::cout << haircut_end_ << std::endl;

                if (VERBOSE) {
                    std::cout << "cur_client_ is empty!" << std::endl;
                    std::cout << "cur_client --> " << cur_client_ << std::endl;
                    std::cout << "haircut_end_ --> " << haircut_end_ << std::endl;
                }
            } else {
                if (VERBOSE) { std::cout << "cur_client_ is not empty!" << std::endl; }

                if (haircut_end_ <= client_time) {
                    if (VERBOSE) { std::cout << ".....haircut_end_ <= client_time!" << std::endl; }

                    std::cout << "printing client time..." << std::endl;
                    std::cout << haircut_end_ << std::endl;

                    if (q_.empty()) {
                        if (VERBOSE) { std::cout << "..........q_ is empty!" << std::endl; }
                        cur_client_ = client;
                        haircut_end_ = client_time + haircut_duration_;
                        if (VERBOSE) {
                            std::cout << "..........cur_client --> " << cur_client_ << std::endl;
                            std::cout << "..........haircut_end_ --> " << haircut_end_ << std::endl;
                        }
                    } else {
                        if (VERBOSE) { std::cout << "..........q_ is not empty!" << std::endl; }

                        cur_client_ = q_.front();
                        haircut_end_ = Time(cur_client_[0], cur_client_[1]) + haircut_duration_;
                        q_.pop();

                        if (VERBOSE) {
                            std::cout << "..........cur_client --> " << cur_client_ << std::endl;
                            std::cout << "..........haircut_end_ --> " << haircut_end_ << std::endl;
                            std::cout << "..........q_ --> " << q_ << std::endl;
                        }
                    }

                } else {
                    if (VERBOSE) { std::cout << ".....haircut_end_ > client_time!" << std::endl; }

                    if (q_.size() + 1 <= client_impatience) {
                        q_.push(client);
                        if (VERBOSE) {
                            std::cout << ".....q_.size() <= client_impatience!" << std::endl;
                            std::cout << ".....q --> " << q_ << std::endl;
                        }

                    } else {
                        if (VERBOSE) { std::cout << "printing client_time..." << std::endl; }
//                        declined_.push(client);
                        std::cout << client_time << std::endl;
                    }
                }

            }

            clients_.pop();
            if (VERBOSE) { std::cout << "clients_ --> " << clients_ << std::endl; }

        }

        std::cout << haircut_end_ << std::endl;

        while (!declined_.empty()) {

        }

        // q_
        while (!q_.empty()) {
            if (VERBOSE) { std::cout << "remainder in q_: " << std::endl; }

            haircut_end_ += haircut_duration_;

            std::cout << haircut_end_ << std::endl;

            q_.pop();

        }

    }


private:

    const size N_;
    std::queue<std::vector<size>> clients_;

    Time global_time_;
    size haircut_duration_;
    Time haircut_end_;
    std::vector<size> cur_client_;
    std::queue<std::vector<size>> q_;
    std::queue<std::vector<size>> declined_;


};

int main () {

    size N = 0;
    std::cin >> N;

    std::queue<std::vector<size>> clients;
    for (size i = 0; i < N; ++i) {
        size hours = 0, minutes = 0, impatience = 0;
        std::cin >> hours >> minutes >> impatience;
        clients.push({hours, minutes, impatience});
    }

    Solution(N, clients).solve();



//    Time time;
//    std::cout << time << std::endl;
//    time += 59;
//    std::cout << time << std::endl;
//    Time time;
//    std::cout << time << std::endl;


    return 0;
}