//
// Created by evasilyev on 20.05.2020.
//

#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <algorithm>
#include <map>

using size = int64_t;

#define VERBOSE true


std::ostream& operator << (std::ostream& os, const std::list<std::map<std::string, size>>& lst) {
    if (lst.empty()) { os << "nullptr"; return os; }

    os << "nullptr -> ";
    size i = 1;
    for (auto& node : lst) {
        os << "#" << i << ": { #" << node.at("id") << ", t = " << node.at("t") << ", a = "
        << node.at("a") << ", b = " << node.at("b") << " } -> ";
        ++i;
    }
    os << "nullptr";

    return os;
}


std::ostream& operator<< (std::ostream& os, const std::vector<std::map<std::string, size>>& v) {
    os << "\n[\n";
    for (auto& e : v) {
        os << "#" << e.at("id") << ", t = " << e.at("t") << ", a = " << e.at("a")
        << ", b = " << e.at("b") << std::endl;
    }
    os << "]\n";

    return os;
}


std::ostream& operator<< (std::ostream& os, const std::queue<std::map<std::string, size>>& q) {
    auto q_copy = q;
    os << "\n";
    for (size i = 0; i < q.size(); ++i) {
        auto front = q_copy.front();
        os << "#" << front.at("id") << ", t = " << front.at("t") << ", a = " << front.at("a")
        << ", b = " << front.at("b") << std::endl;
        q_copy.pop();
    }

    return os;
}


class Solution {
public:
    Solution(size n, size m, std::vector<std::vector<size>> clients)
    : n_(n), m_(m), clients_(std::move(clients)), at_employees_cnt_(0) {
        clients_.push_back({{1000000001, 0, 0}});
    }

    std::string solve() {
        std::string answer;

        size id = 0;
        for (auto& client : clients_) {
            if (VERBOSE) { std::cout << "===========================================" << std::endl; }

            auto t = client[0];
            auto a = client[1];
            auto b = client[2];

            if (VERBOSE) { std::cout << "client #" << id << ": t = " << t << ", a = " << a << ", b = " << b << std::endl; }

            // add client to queue to employees
            q_employees_.push({{"id", id}, {"t", t}, {"a", a}, {"b", b}});

            if (VERBOSE) { std::cout << "q_employees_ --> " << q_employees_ << std::endl; }

            // get clients ready after employees and sort them
            std::vector<std::map<std::string, size>> ready_first;
            auto it = at_employees_.begin();
            while (it != at_employees_.end()) {
                if ((*it).at("t") <= t) {
                    ready_first.push_back({{"id", id}, {"t", t}, {"a", a}, {"b", b}});

                }
                if (!q_employees_.empty()) {
                    *it = q_employees_.front();
                    q_employees_.pop();
                }

                it = at_employees_.erase(it);
                it++;
            }
            if (VERBOSE) { std::cout << "ready_first = " << ready_first << std::endl; }
            if (ready_first.empty()) { ++id; continue; }

            // sort nonempty ready_first
            std::sort(ready_first.begin(), ready_first.end(),
                [](const std::map<std::string, size>& lhs, const std::map<std::string, size>& rhs){
                if (lhs.at("t") == rhs.at("t")) { return lhs.at("id") < rhs.at("id"); }
                else { return lhs.at("t") < rhs.at("t"); }
            });
            if (VERBOSE) { std::cout << "sorted ready_first --> " << ready_first << std::endl; }

            // update ready_first times with +b
            size t_last = ready_first[0].at("t");
            if (!ready_clients_.empty()) {
                size n = ready_first.size();
                t_last = ready_clients_[n - 1].at("t");
            }
            for (auto& cl : ready_first) {
                t_last += cl.at("b");
                cl.at("b") = t_last;
            }
            if (VERBOSE) { std::cout << "updated with +b ready_first --> " << ready_first << std::endl; }

            // get clients to employees
            size i = 0;
            while (at_employees_cnt_ < m_ && !q_employees_.empty()) {
                auto front = q_employees_.front();
                front.at("t") = ready_first[i++].at("t") + front.at("a");
                at_employees_.push_back(front);
                q_employees_.pop();
            }
            if (VERBOSE) { std::cout << "at_employees_cnt_ = " << at_employees_cnt_ << ", at_employees_ --> "
            << at_employees_ << std::endl; }

            // increase clients id
            ++id;
        }

        // sort ready clients by id
        std::sort(ready_clients_.begin(), ready_clients_.end(),
                [](const std::map<std::string, size>& lhs, const std::map<std::string, size>& rhs){
            return lhs.at("id") < rhs.at("id");
        });
        if (VERBOSE) { std::cout << "sorted ready_clients_ --> " << ready_clients_ << std::endl; }

        // compose answer
        for (auto& cl : ready_clients_) { answer += std::to_string(cl.at("t")) + "\n"; }

        return answer;
    }

private:

    const size n_;
    const size m_;
    std::vector<std::vector<size>> clients_;

    std::list<std::map<std::string, size>> at_employees_;
    size at_employees_cnt_;
    std::queue<std::map<std::string, size>> q_employees_;

    std::vector<std::map<std::string, size>> ready_clients_;
};


int main() {

    size n = 0, m = 0;
    std::cin >> n >> m;

    std::vector<std::vector<size>> clients;
    for (size i = 0; i < n; ++i) {
        size t = 0, a = 0, b = 0;
        std::cin >> t >> a >> b;
        clients.push_back({t, a, b});
    }

    std::cout << Solution(n, m, clients).solve();



    return 0;
}