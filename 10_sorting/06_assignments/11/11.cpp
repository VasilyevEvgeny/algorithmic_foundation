#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

#define VERBOSE false


enum class PointType {
    START,
    END,
    POINT,
};

std::ostream& operator << (std::ostream& os, const std::vector<std::pair<int, PointType>>& v) {
    os << "[ ";
    for (int i = 0; i < v.size(); ++i) {
        os << "{" << v[i].first << ", ";
        if (v[i].second == PointType::START) {
            os << "START";
        } else if (v[i].second == PointType::END) {
            os << "END";
        } else if (v[i].second == PointType::POINT) {
            os << "POINT";
        }
        os << "}";
        if (i != v.size() - 1) {
            os << " ";
        }
    }
    os << " ]";
    return os;
}

template <typename T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& v) {
    os << "[ ";
    for (int i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i != v.size() - 1) {
            os << " ";
        }
    }
    os << " ]";
    return os;
}

template <typename T>
std::ostream& operator << (std::ostream& os, const std::map<T, T>& m) {
    os << "{ ";
    for (const auto& e : m) {
        os << "{" << e.first << ", " << e.second << "} ";
    }
    os << "}";
    return os;
}




class Solution {
public:
    Solution(const std::vector<std::pair<int, PointType>>& pool, const std::vector<int>& points)
            : pool_(pool)
            , points_(points) {

        if (VERBOSE) {
            std::cout << pool_ << std::endl;
            std::cout << points_ << std::endl;
        }

        std::sort(pool_.begin(), pool_.end(),
                  [](const std::pair<int, PointType>& lhs,
                     const std::pair<int, PointType>& rhs){

                      if (lhs.first == rhs.first) {
                          if (lhs.second == PointType::POINT && rhs.second == PointType::START) {
                              return false;
                          } else if (lhs.second == PointType::START && rhs.second == PointType::POINT) {
                              return true;
                          } else if (lhs.second == PointType::POINT && rhs.second == PointType::END) {
                              return true;
                          } else if (lhs.second == PointType::END && rhs.second == PointType::POINT) {
                              return false;
                          } else if (lhs.second == PointType::START && rhs.second == PointType::END) {
                              return true;
                          } else if (lhs.second == PointType::END && rhs.second == PointType::START) {
                              return false;
                          } else {
                              return false;
                          }
                      } else {
                          return lhs.first < rhs.first;
                      }



                  });

        if (VERBOSE) {
            std::cout << pool_ << std::endl;
        }

        std::map<int, int> info;
        int n_start = 0, n_end = 0;
        for (int i = 0; i < pool_.size(); ++i) {
            if (pool_[i].second == PointType::START) {
                ++n_start;
            } else if (pool_[i].second == PointType::END) {
                ++n_end;
            } else {
                info.insert({pool_[i].first, n_start - n_end});
            }
        }

        if (VERBOSE) {
            std::cout << info << std::endl;
        }

        for (int i = 0; i < points.size(); ++i) {
            std::cout << info.at(points[i]) << " ";
        }
        std::cout << std::endl;


    }


private:
    std::vector<std::pair<int, PointType>> pool_;
    const std::vector<int> points_;

};



int main() {

    int n = 0, m = 0;
    std::cin >> n >> m;

    std::vector<std::pair<int, PointType>> pool;
    for (int i = 0; i < n; ++i) {
        int a = 0, b = 0;
        std::cin >> a >> b;
        pool.push_back({std::min(a,b), PointType::START});
        pool.push_back({std::max(a,b), PointType::END});
    }

    std::vector<int> points(m);
    for (int i = 0; i < m; ++i) {
        int point = 0;
        std::cin >> point;
        pool.push_back({point, PointType::POINT});
        points[i] = point;
    }

    Solution solution(pool, points);


    return 0;
}