//
// Created by evasilyev on 09.02.2020.
//

#include <iostream>
#include <vector>
#include <string>

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
        just_loaded = true;
    }

    void reset_load_flag() {
        just_loaded = false;
    }

    std::vector<Paper> process_epoch() {
        if (VERBOSE) {std::cout << "process_epoch for " << name << std::endl;}

        just_loaded = false;

        t = std::max(0, t - 1);
        if (t) {
            return {};
        }
        else {
            return {Paper(), Paper()};
        }
    }

    bool is_ready() const {
        return t == 0;
    }

    bool is_just_loaded() const {
        return just_loaded;
    }

    //
    // properties
    //

    int get_t() const {
        return t;
    }

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
    bool just_loaded = false;
};

void process_copier(Copier& copier, std::vector<Paper>& papers) {
    if (!copier.is_just_loaded()) {
        auto result = copier.process_epoch();
        std::copy(result.begin(), result.end(), std::back_inserter(papers));
    }
}

int main() {

    int N = 0, x = 0, y = 0;
    std::cin >> N >> x >> y;

    auto Copier_1 = Copier("first", x);
    auto Copier_2 = Copier("second", y);

    auto papers = std::vector<Paper>({Paper()});

    int t_epoch = 0;

    while (papers.size() < N) {
        t_epoch += 1;

        Copier_1.reset_load_flag();
        Copier_2.reset_load_flag();

        if (VERBOSE) {std::cout << "**********\nt_epoch = " << t_epoch << ", papers.size() = " << papers.size() << std::endl;}

        // loading

        if (papers.size() <= 1) {
            if (Copier_1.is_ready() && Copier_2.is_ready()) {
                (Copier_1.get_duration() <= Copier_2.get_duration()) ? Copier_1.load_paper(papers) : Copier_2.load_paper(papers);
            }
            else if (Copier_1.is_ready()) {
                Copier_1.load_paper(papers);
            }
            else {
                Copier_2.load_paper(papers);
            }
        }
        else if (papers.size() > 1) {
            if (Copier_1.is_ready()) {
                Copier_1.load_paper(papers);
            }
            if (Copier_2.is_ready()) {
                Copier_2.load_paper(papers);
            }
        }

        // process

        process_copier(Copier_1, papers);
        process_copier(Copier_2, papers);

        if (VERBOSE) {
            Copier_1.print_state();
            Copier_2.print_state();
        }

        if (VERBOSE) {std::cout << "papers.size() = " << papers.size() << std:: endl;}
    }

    if (VERBOSE) {std::cout << "**********" << std::endl;}
    std::cout << t_epoch << std::endl;

    return 0;
}