#include <iostream>
#include <deque>
#include <algorithm>

#define MODE 1
#define VERBOSE false


using size = uint64_t;


template <typename T>
std::ostream& operator << (std::ostream& os, const std::deque<T>& v) {
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

class Solution {
public:
    Solution(std::deque<std::deque<size>> commands)
        : commands_(std::move(commands)) {

        counting_sort();


    }

    void counting_sort() {
        std::deque<std::deque<std::deque<size>>> cnt(MAX_S, std::deque<std::deque<size>>());
        for (const auto& command : commands_) {
            if (VERBOSE) {
                std::cout << "...command: " << command << std::endl;
            }
            cnt[command[0]].push_back(command);
        }

        if (VERBOSE) {
            std::cout << "Cnt array:" << std::endl;
            for (const auto& s_commands : cnt) {
                std::cout << "[ ";
                for (const auto& command : s_commands) {
                    std::cout << command << " ";
                }
                std::cout << " ]" << std::endl;
            }
        }


        for (auto& s_command : cnt) {
            std::sort(s_command.begin(),
                      s_command.end(),
                      [](const std::deque<size>& lhs, const std::deque<size>& rhs){
                if (lhs[1] == rhs[1]) {
                    return lhs[2] < rhs[2];
                } else {
                    return lhs[1] < rhs[1];
                }
            });
        }



        if (VERBOSE) {
            std::cout << "Cnt array sorted:" << std::endl;
            for (const auto& s_commands : cnt) {
                std::cout << "[ ";
                for (const auto& command : s_commands) {
                    std::cout << command << " ";
                }
                std::cout << " ]" << std::endl;
            }
        }

        for (int64_t i = cnt.size() - 1, k = 0; i >= 0; --i) {
            while (!cnt[i].empty()) {
                commands_[k++] = cnt[i][0];
                cnt[i].pop_front();
            }
        }

        if (VERBOSE) {
            std::cout << "Sorted data:" << std::endl;
            for (const auto& e : commands_) {
                std::cout << e << std::endl;
            }
        }

        for (const auto& e : commands_) {
            std::cout << e[2] + 1 << " ";
        }
        std::cout << std::endl;
    }

private:
    std::deque<std::deque<size>> commands_;

    const size MAX_S = 101;
};


int main() {

    size_t N = 0;
    std::cin >> N;

    std::deque<std::deque<size>> commands(N);
    for (size_t i = 0; i < N; ++i) {
        std::deque<size> command(3);
        for (size_t j = 0; j < 2; ++j) {
            std::cin >> command[j];
        }
        command[2] = i;
        if (VERBOSE) {
            std::cout << "command: " << command << std::endl;
        }
        commands[i] = command;
    }

    if (VERBOSE) {
        std::cout << "Initial data:" << std::endl;
        for (const auto& e : commands) {
            std::cout << e << std::endl;
        }
    }

    Solution solution(commands);

    return 0;
}