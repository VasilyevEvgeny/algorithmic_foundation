//
// Created by evasilyev on 09.02.2020.
//

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cassert>

// 1 - work, 2 - manual tests
#define MODE 2

#define VERBOSE false

class Paper {
public:
    Paper() = default;
    ~Paper() = default;
};

class Copier {
public:
    Copier(std::string _name, const int _duration)
    : name(std::move(_name))
    , duration(_duration) {};

    ~Copier() = default;

    //
    // functions
    //

    void load_paper(std::vector<Paper>& papers) {
        papers.pop_back();

        if (VERBOSE) {std::cout << "paper has been loaded to " << name << std::endl;}

        t = duration;
    }

    std::vector<Paper> process_epoch(int dt) {
        if (VERBOSE) {std::cout << "process_epoch for " << name << std::endl;}

        t = std::max(0, t - dt);
        if (t) {
            return {};
        }
        else {
            return {Paper(), Paper()};
        }
    }

    bool is_ready() const {
        return !t;
    }

    //
    // properties
    //

    int get_duration() const {
        return duration;
    }

    void print_state() const {
        std::cout << name << ": duration = " << duration << ", t = " << t << ", ready = ";
       ((is_ready()) ? std::cout << "true" : std::cout << "false") << std::endl;
    }

private:

    // parameters

    const std::string name;
    const int duration;
    int t = 0;
};

void process_copier(Copier& copier, std::vector<Paper>& papers, int dt) {
    auto result = copier.process_epoch(dt);
    std::copy(result.begin(), result.end(), std::back_inserter(papers));
}

void load(Copier& copier_1, Copier& copier_2, std::vector<Paper>& papers) {
    if (papers.size() <= 1) {
        if (copier_1.is_ready() && copier_2.is_ready()) {
            (copier_1.get_duration() <= copier_2.get_duration()) ? copier_1.load_paper(papers) : copier_2.load_paper(papers);
        }
        else if (copier_1.is_ready()) {
            copier_1.load_paper(papers);
        }
        else {
            copier_2.load_paper(papers);
        }
    }
    else {
        if (copier_1.is_ready()) {
            copier_1.load_paper(papers);
        }
        if (copier_2.is_ready()) {
            copier_2.load_paper(papers);
        }
    }
}

int straight(int N, int x, int y) {
    auto copier_1 = Copier("1st", x);
    auto copier_2 = Copier("2nd", y);

    int dt = std::min(copier_1.get_duration(), copier_2.get_duration());

    auto papers = std::vector<Paper>({Paper()});

    load(copier_1, copier_2, papers);

    int t_epoch = 0;
    while (papers.size() <= N) {
        if (VERBOSE) {std::cout << "papers.size() = " << papers.size() << std:: endl;}
        if (VERBOSE) { copier_1.print_state(); copier_2.print_state(); }

        // process

        process_copier(copier_1, papers, dt);
        process_copier(copier_2, papers, dt);

        if (VERBOSE) { copier_1.print_state(); copier_2.print_state(); }

        load(copier_1, copier_2, papers);

        if (VERBOSE) { copier_1.print_state(); copier_2.print_state(); }

        t_epoch += dt;

        if (VERBOSE) {std::cout << "**********\nt_epoch = " << t_epoch << ", papers.size() = " << papers.size() << std::endl;}
    }

    return t_epoch;
}

void manual_tests() {
    std::vector<std::map<std::string, int>> test_data = {

            // N, 1, 1
            {{"N", 1}, {"x", 1}, {"y", 1}, {"t", 1}},
            {{"N", 2}, {"x", 1}, {"y", 1}, {"t", 2}},
            {{"N", 3}, {"x", 1}, {"y", 1}, {"t", 2}},
            {{"N", 4}, {"x", 1}, {"y", 1}, {"t", 3}},
            {{"N", 5}, {"x", 1}, {"y", 1}, {"t", 3}},
            {{"N", 6}, {"x", 1}, {"y", 1}, {"t", 4}},
            {{"N", 7}, {"x", 1}, {"y", 1}, {"t", 4}},
            {{"N", 8}, {"x", 1}, {"y", 1}, {"t", 5}},
            {{"N", 9}, {"x", 1}, {"y", 1}, {"t", 5}},
            {{"N", 10}, {"x", 1}, {"y", 1}, {"t", 6}},

            // 1, x, y
            {{"N", 1}, {"x", 1}, {"y", 2}, {"t", 1}},
            {{"N", 1}, {"x", 2}, {"y", 2}, {"t", 2}},
            {{"N", 1}, {"x", 3}, {"y", 2}, {"t", 2}},
            {{"N", 1}, {"x", 3}, {"y", 3}, {"t", 3}},
            {{"N", 1}, {"x", 10}, {"y", 3}, {"t", 3}},
            {{"N", 1}, {"x", 3}, {"y", 10}, {"t", 3}},
            {{"N", 1}, {"x", 10}, {"y", 10}, {"t", 10}},

            // simple
            {{"N", 2}, {"x", 1}, {"y", 2}, {"t", 2}},
            {{"N", 2}, {"x", 2}, {"y", 1}, {"t", 2}},
            {{"N", 2}, {"x", 2}, {"y", 2}, {"t", 4}},
            {{"N", 2}, {"x", 2}, {"y", 10}, {"t", 4}},
            {{"N", 2}, {"x", 10}, {"y", 2}, {"t", 4}},
            {{"N", 3}, {"x", 1}, {"y", 2}, {"t", 3}},
            {{"N", 3}, {"x", 2}, {"y", 1}, {"t", 3}},
            {{"N", 5}, {"x", 1}, {"y", 2}, {"t", 4}},
            {{"N", 5}, {"x", 2}, {"y", 1}, {"t", 4}},

    };

    for (const auto& line : test_data) {
        int N = line.at("N"), x = line.at("x"), y = line.at("y");
        auto t_true = line.at("t");
        auto t_pred = straight(N, x, y);

        std::cerr << "N = " << N << ", x = " << x << ", y = " << y <<  ", t_true = " << t_true << ", t_pred = " << t_pred << std::endl;
        assert(t_true == t_pred);

    }
}

int main() {

#if MODE == 1
    int N = 0, x = 0, y = 0;
    std::cin >> N >> x >> y;

    auto res = straight(N, x, y);

    if (VERBOSE) {std::cout << "**********" << std::endl;}
    std::cout << res << std::endl;
#elif MODE == 2
    manual_tests();
#elif MODE == 3

#endif


    return 0;
}