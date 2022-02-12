#include <iostream>
#include <vector>

const int kInfinity = 1<<30;

struct DpCell {
    int sum = 0;
    std::vector<unsigned short> mails;
};

std::pair<int, std::vector<unsigned short>> findAnswer(unsigned short villageAmount, unsigned short mailAmount,
                                                       std::vector<unsigned short>& villages) {
    DpCell dp[villageAmount + 1][mailAmount + 1];
    dp[0][1].sum = 0;
    for (unsigned short i = 1; i <= villageAmount; ++i) {
        dp[i][1].sum = dp[i - 1][1].sum + (i - 1) * (villages[i] - villages[i - 1]);
        dp[i][1].mails.push_back(i);
    }
    for (unsigned short i = 2; i <= mailAmount; ++i) {
        for (unsigned short j = i; j <= villageAmount; ++j) {
            int minSum = kInfinity; //dp[j][i]
            unsigned short mailIndex = j - 1;
            for (unsigned short k = 1; k <= j - i + 1; ++k) {
                int newSum = dp[j - k][i - 1].sum;
                for (unsigned short u = 0; u < k; ++u) {
                    newSum += std::min(abs(villages[j - u] - villages[j]), abs(villages[j - u] - villages[j - k]));
                }
                if (newSum < minSum) {
                    minSum = newSum;
                    mailIndex = j - k;
                }
            }
            dp[j][i].sum = minSum;
            for (size_t k = 0; k < dp[mailIndex][i - 1].mails.size(); ++k) {
                dp[j][i].mails.push_back(dp[mailIndex][i - 1].mails[k]);
            }
            dp[j][i].mails.push_back(j);
        }
    }
    int minSum = kInfinity;
    unsigned short minIndex = 1;
    for (unsigned short i = 1; i <= villageAmount; ++i) {
        int newSum = dp[i][mailAmount].sum;
        for (unsigned short j = i + 1; j <= villageAmount; ++j) {
            newSum += (villages[j] - villages[i]);
        }
        if (newSum < minSum) {
            minSum = newSum;
            minIndex = i;
        }
    }
    return std::make_pair(minSum, dp[minIndex][mailAmount].mails);
}


int main() {
    unsigned short villageAmount;
    unsigned short mailAmount;
    std::cin >> villageAmount >> mailAmount;
    std::vector<unsigned short> villages(villageAmount);
    villages.push_back(0);
    for (unsigned short i = 1; i <= villageAmount; ++i) {
        std::cin >> villages[i];
    }
    std::pair<int, std::vector<unsigned short>> answer = findAnswer(villageAmount, mailAmount, villages);
    std::cout << answer.first << '\n';
    for (unsigned short i = 0; i < mailAmount; ++i) {
        std::cout << villages[answer.second[i]] << " ";
    }
}
