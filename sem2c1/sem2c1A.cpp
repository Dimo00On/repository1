#include <iostream>
#include <vector>
#include <algorithm>

const int kInfinity = 1 << 30;
const int kMaxGirlsInTaxi = 3;
const int kTaxiSize = 4;

struct DpCell{
    int sum;
    std::pair<int, int> previous;
    std::vector<std::string> taxiPack;
};

void buildBase(std::vector<std::pair<std::string, int>>& boys, int amountBoysNextDoor,
               std::vector<std::pair<std::string, int>>& girls, int amountGirls,
               std::vector<std::vector<DpCell>>& dp) {
    for (int i = 0; i <= amountBoysNextDoor; ++i) {
        dp[i].resize(amountGirls + 1);
        for (int j = 0; j <= amountGirls; ++j) {
            dp[i][j].sum = kInfinity;
        }
    }
    dp[0][0].sum = 0;
    dp[0][0].previous = std::make_pair(0,0);
}

int findAnswer(std::vector<std::pair<std::string, int>>& boys, int amountBoysNextDoor,
                std::vector<std::pair<std::string, int>>& girls, int amountGirls,
                std::vector<std::vector<std::string>>& answer) {
    std::vector<std::vector<DpCell>> dp(amountBoysNextDoor + 1);
    buildBase(boys, amountBoysNextDoor, girls, amountGirls, dp);
    for (int i = 0; i <= amountGirls; ++i) {
        for (int j = 0; j < amountBoysNextDoor; ++j) {
            for (int u = 1; u <= kTaxiSize; ++u) {
                int boyIndex = j + u;
                int girlIndex = i + kTaxiSize - u;
                int price = boys[j].second;
                if (u != 0 && i != amountGirls && girls[i].second > price) {
                    price = girls[i].second;
                }
                int newSum = dp[j][i].sum + price;
                if (boyIndex > amountBoysNextDoor && girlIndex >= amountGirls) {
                    boyIndex = amountBoysNextDoor;
                    girlIndex = amountGirls;
                } else {
                    if (boyIndex > amountBoysNextDoor || girlIndex > amountGirls) {
                        continue;
                    }
                }
                if (newSum < dp[boyIndex][girlIndex].sum) {
                    dp[boyIndex][girlIndex].sum = newSum;
                    dp[boyIndex][girlIndex].previous = std::make_pair(j, i);
                    dp[boyIndex][girlIndex].taxiPack.clear();
                    for (int h = j; h < boyIndex; ++h) {
                        dp[boyIndex][girlIndex].taxiPack.push_back(boys[h].first);
                    }
                    for (int h = i; h < girlIndex; ++h) {
                        dp[boyIndex][girlIndex].taxiPack.push_back(girls[h].first);
                    }
                }
            }
        }
    }
    DpCell minimal = dp[amountBoysNextDoor][amountGirls];
    std::pair<int, int> index = minimal.previous;
    answer.push_back(minimal.taxiPack);
    while (index.first + index.second >= kTaxiSize) {
        answer.push_back(dp[index.first][index.second].taxiPack);
        index = dp[index.first][index.second].previous;
    }
    return minimal.sum;
}

bool comparator(std::pair<std::string, int>& first, std::pair<std::string, int>& second) {
    return first.second > second.second;
}

int main() {
    int amountBoysNextDoor;
    int amountGirls;
    std::cin >> amountBoysNextDoor;
    std::vector<std::pair<std::string, int>> boys(amountBoysNextDoor);
    for (int i = 0; i < amountBoysNextDoor; ++i) {
        std::cin >> boys[i].first >> boys[i].second;
    }
    std::cin >> amountGirls;
    std::vector<std::pair<std::string, int>> girls(amountGirls);
    for(int i = 0; i < amountGirls; ++i) {
        std::cin >> girls[i].first >> girls[i].second;
    }
    std::sort(boys.begin(), boys.end(), comparator);
    std::sort(girls.begin(), girls.end(), comparator);
    if (amountBoysNextDoor + amountGirls <= kTaxiSize) {
        if (amountGirls > 0) {
            std::cout << std::max(girls[0].second, boys[0].second) << '\n';
        } else {
            std::cout << boys[0].second << '\n';
        }
        std::cout << 1 << '\n';
        std::cout << "Taxi 1: ";
        std::vector<std::string> answer;
        for (int i = 0; i < amountGirls; ++i) {
            answer.push_back(girls[i].first);
        }
        for (int i = 0; i < amountBoysNextDoor; ++i) {
            answer.push_back(boys[i].first);
        }
        if (answer.size() > 1) {
            for (size_t j = 0; j < answer.size() - 1; ++j) {
                std::cout << answer[j];
                if (j != answer.size() - 2) {
                    std::cout << ", ";
                } else {
                    std::cout << " and ";
                }
            }
            std::cout << answer[answer.size() - 1] << "." << '\n';
        } else {
            std::cout << answer[0] << "." << '\n';
        }
        return 0;
    }
    std::vector<std::vector<std::string>> answer;
    int sum = findAnswer(boys, amountBoysNextDoor, girls, amountGirls, answer);
    std::cout << sum << '\n' << answer.size() << '\n';
    for (size_t i = 0; i < answer.size(); ++i) {
        std::cout << "Taxi " << i + 1 << ": ";
        if (answer[i].size() > 1) {
            for (size_t j = 0; j < answer[i].size() - 1; ++j) {
                std::cout << answer[i][j];
                if (j != answer[i].size() - 2) {
                    std::cout << ", ";
                } else {
                    std::cout << " and ";
                }
            }
            std::cout << answer[i][answer[i].size() - 1] << "." << '\n';
        } else {
            std::cout << answer[i][0] << "." << '\n';
        }
    }
}
