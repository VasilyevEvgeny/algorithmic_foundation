#include <iostream>
#include <map>
#include <vector>
#include <algorithm>


enum class PointType {
    START,
    END,
    POINT,
};

std::ostream& operator << (std::ostream& os, const std::vector<std::pair<int, PointType>>& v) {
    os << "[ ";
    for (size_t i = 0; i < v.size(); ++i) {
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






class Solution {
public:
    Solution(const std::vector<std::pair<int, PointType>>& pool)
            : pool_(pool) {

        std::cout << pool_ << std::endl;

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
                          }
                      } else {
                          return lhs.first < rhs.first;
                      }



                  });

        std::cout << pool_ << std::endl;


        for (size_t i = 0; i < pool_.size(); ++i) {


        }



    }


private:
    std::vector<std::pair<int, PointType>> pool_;

};



int main() {

    size_t n = 0, m = 0;
    std::cin >> n >> m;

    std::vector<std::pair<int, PointType>> pool;
    for (size_t i = 0; i < n; ++i) {
        int start = 0, end = 0;
        std::cin >> start >> end;
        std::cout << start << " " << end << std::endl;
        pool.push_back({start, PointType::START});
        pool.push_back({end, PointType::END});
    }

    for (size_t i = 0; i < m; ++i) {
        int point = 0;
        std::cin >> point;
        pool.push_back({point, PointType::POINT});
    }

    Solution solution(pool);


    return 0;
}ы