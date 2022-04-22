#include <iostream>
#include <sstream>
#include <stack>
#include <map>

#define VERBOSE true

using size = int64_t;

class StackWithMin {
public:
    StackWithMin() = default;

    void get_command(const std::string& command) {
        std::istringstream ss(command);
        std::string operation;
        size argument = 0;
        ss >> operation;
        if (ss) {
            ss >> argument;
        }

        if (operation == "push") {
            push(argument);
        }
        else if (operation == "pop") {
            pop();
        }
        else if (operation == "min") {
            min();
        }
    }


private:

    void push(size val) {
        if (stack_.empty()) {
            stack_.push({val, val});
        }
        else {
            size current_min = stack_.top().second;
            stack_.push({val, std::min(current_min, val)});
        }
    }

    void pop() {
        stack_.pop();
    }

    void min() const {
        std::cout << stack_.top().second << std::endl;
    }

    std::stack<std::pair<size, size>> stack_;
};

int main() {

    StackWithMin stackWithMin;

    size q = 0;
    std::cin >> q;
    std::cin.ignore();
    for (size i = 0; i < q; ++i) {
        std::string command;
        std::getline(std::cin, command);
        stackWithMin.get_command(command);
    }

    return 0;
}