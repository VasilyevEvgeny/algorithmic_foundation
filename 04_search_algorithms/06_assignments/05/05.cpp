#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <random>
#include <cassert>

// 1 - work, 2 - manual testing, 3 - stress testing
#define MODE 1

#define VERBOSE false

using integer = int64_t;

//
// straight
//

class Paper {
public:
    Paper() = default;
    ~Paper() = default;
};

class Copier {
public:
    Copier() : name(""), duration(0) {};

    Copier(std::string _name, const integer _duration)
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

    std::vector<Paper> process(integer dt) {
        if (VERBOSE) {std::cout << "process_epoch for " << name << std::endl;}

        integer t_prev = t;
        t = std::max((integer)0, t - dt);
        if (t == 0 && t_prev > 0) {
            return {Paper(), Paper()};
        }
        else {
            return {};
        }
    }

    bool is_ready() const {
        return !t;
    }

    //
    // properties
    //

    integer get_duration() const {
        return duration;
    }

    void print_state() const {
        std::cout << name << ": duration = " << duration << ", t = " << t << ", ready = ";
       ((is_ready()) ? std::cout << "true" : std::cout << "false") << std::endl;
    }

private:

    // parameters

    std::string name;
    integer duration;
    integer t = 0;
};

class StraightSolver {
public:
    StraightSolver()
    : N(0), x(0), y(0) {
        copier_1 = Copier();
        copier_2 = Copier();
        dt = 0; };

    StraightSolver(integer _N, integer _x, integer _y)
    : N(_N)
    , x(_x)
    , y(_y) {
        copier_1 = Copier("1st", x);
        copier_2 = Copier("2nd", y);

        dt = 1;

        papers = std::vector<Paper>({Paper()});

    };

    ~StraightSolver() {
        papers.clear();
    }

    integer count_papers() {
        integer in_copiers = 0;
        if (!copier_1.is_ready()) { in_copiers++; }
        if (!copier_2.is_ready()) { in_copiers++; }
        return in_copiers + papers.size();
    }

    integer solve() {

        loading();
        if (VERBOSE) { std::cout << "*************" << std::endl; }

        integer t_epoch = 0;
        while (count_papers() <= N) {
            t_epoch += dt;

            if (VERBOSE) { std::cout << "papers = " << count_papers() << std:: endl; }
            if (VERBOSE) { copier_1.print_state(); copier_2.print_state(); }

            // process

            process_copier(copier_1);
            process_copier(copier_2);

            if (VERBOSE) { copier_1.print_state(); copier_2.print_state(); }

            loading();

            if (VERBOSE) { copier_1.print_state(); copier_2.print_state(); }

            if (VERBOSE) { std::cout << "t_epoch = " << t_epoch << ", count_papers() = " << count_papers() <<
            "\n*************" <<std::endl; }
        }

        return t_epoch;
    }

private:

    void process_copier(Copier& copier) {
        auto result = copier.process(dt);
        std::copy(result.begin(), result.end(), std::back_inserter(papers));
    }

    void loading() {
        if (papers.size() == 1) {
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
        else if (papers.size() > 1) {
            if (copier_1.is_ready()) {
                copier_1.load_paper(papers);
            }
            if (copier_2.is_ready()) {
                copier_2.load_paper(papers);
            }
        }
    }

    const integer N, x, y;
    Copier copier_1, copier_2;
    integer dt = 0;
    std::vector<Paper> papers;
};

//
// binary
//

class BinarySolver {
public:
    BinarySolver(integer _N, integer _x, integer _y)
    : N(_N), x(_x), y(_y) {};

    integer solve() {
        integer min_time = std::min(x, y);
        integer l = min_time;
        integer r = N * min_time;

        while (l <= r) {
            integer m = l + (r - l) / 2;

            if (is_time_valid(m - min_time)) {
                l = m + 1;
            }
            else {
                r = m - 1;
            }
        }

        return  r + 1;
    }

private:

    bool is_time_valid(integer t) {
        return t / x + t / y < N - 1;
    }

    const integer N, x, y;
};


void manual_testing() {
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
        auto t_pred_straight = StraightSolver(N, x, y).solve();
        auto t_pred_binary = BinarySolver(N, x, y).solve();

        std::cerr << "N = " << N << ", x = " << x << ", y = " << y <<  ", t_true = " << t_true << ", t_pred_straight = "
        << t_pred_straight << ", t_pred_binary = " << t_pred_binary << std::endl;

        assert(t_true == t_pred_straight);
        assert(t_true == t_pred_binary);

    }
}


void stress_testing() {
    std::random_device rd;
    std::mt19937 gen(rd());

    const integer MIN_N = 1, MAX_N = 100;
    const integer MIN_DURATION = 1, MAX_DURATION = 10;

    std::uniform_int_distribution<> dist_N(MIN_N, MAX_N);
    std::uniform_int_distribution<> dist_duration(MIN_DURATION, MAX_DURATION);

    integer N, x, y;
    while (true) {
        N = dist_N(gen);
        x = dist_duration(gen);
        y = dist_duration(gen);

        auto res_straight = StraightSolver(N, x, y).solve();
        auto res_binary = BinarySolver(N, x, y).solve();

        std::cout << "N = " << N << ", x = " << x << ", y = " << y << ", res_straight = " << res_straight <<", res_binary = " << res_binary << std::endl;
        assert(res_straight == res_binary);
    }
}


int main() {

#if MODE == 1
    int N = 0, x = 0, y = 0;
    std::cin >> N >> x >> y;

//    auto res = StraightSolver(N, x, y).solve();
    auto res = BinarySolver(N, x, y).solve();

    std::cout << res << std::endl;
#elif MODE == 2
    manual_testing();
#elif MODE == 3
    stress_testing();
#endif


    return 0;
}