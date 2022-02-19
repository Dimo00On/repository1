#include <iostream>
#include <vector>
#include <string>

struct DpCell {
    unsigned short length;
    char letter = ' ';
    std::pair<unsigned short, unsigned short> nextCell;
    bool isNewLetterAdd = false;
};

std::string simpleAnswer(std::string& first, std::string& second, unsigned short size1, unsigned short size2) {
    DpCell dp[size1 + 1][size2 + 1];
    for (unsigned short i = 0; i <= size1; ++i) {
        dp[i][0].length = 0;
    }
    for (unsigned short i = 0; i <= size2; ++i) {
        dp[0][i].length = 0;
    }
    for (unsigned short i = 1; i <= size2; ++i) {
        for (unsigned short j = 1; j <= size1; ++j) {
            if (dp[j - 1][i].length > dp[j][i - 1].length) {
                dp[j][i] = dp[j - 1][i];
                dp[j][i].nextCell = (dp[j - 1][i].isNewLetterAdd ? std::make_pair((unsigned short)(j - 1), i)
                                                                 : dp[j - 1][i].nextCell);
            } else {
                dp[j][i] = dp[j][i - 1];
                dp[j][i].nextCell = (dp[j][i - 1].isNewLetterAdd ? std::make_pair(j, (unsigned short)(i - 1))
                                                                 : dp[j][i - 1].nextCell);
            }
            dp[j][i].isNewLetterAdd = false;
            if (first[j - 1] == second[i - 1] && dp[j - 1][i - 1].length + 1 > dp[j][i].length) {
                dp[j][i].length = dp[j - 1][i - 1].length + 1;
                dp[j][i].letter = first[j - 1];
                dp[j][i].nextCell = (dp[j - 1][i - 1].isNewLetterAdd ? std::make_pair((unsigned short)(j - 1), (unsigned short)(i - 1))
                                                                     : dp[j - 1][i - 1].nextCell);
                dp[j][i].isNewLetterAdd = true;
            }
        }
    }
    std::vector<char> answerVector;
    int length = dp[size1][size2].length;
    if (dp[size1][size2].isNewLetterAdd) {
        answerVector.push_back(dp[size1][size2].letter);
        --length;
    }
    std::pair<unsigned short, unsigned short> position = dp[size1][size2].nextCell;
    while (length > 0) {
        answerVector.push_back(dp[position.first][position.second].letter);
        position = dp[position.first][position.second].nextCell;
        --length;
    }
    std::string answerString;
    for (size_t i = answerVector.size(); i > 0; --i) {
        answerString += answerVector[i - 1];
        answerVector.pop_back();
    }
    return answerString;
}

std::string smartAnswer(std::string& first, std::string& second, unsigned short size1, unsigned short size2) {
    if (size1 < 100) {
        return simpleAnswer(first, second, size1, size2);
    }
    unsigned short middleFirst = size1 / 2;
    unsigned short* dpLeft = new unsigned short[size2 + 1];
    for (int j = 0; j <= size2; ++j) {
        dpLeft[j] = 0;
    }
    for (unsigned short i = 1; i <= middleFirst; ++i) {
        unsigned short* dpLeftNew = new unsigned short[size2 + 1];
        dpLeftNew[0] = 0;
        for (unsigned short j = 1; j <= size2; ++j) {
            dpLeftNew[j] = std::max(dpLeft[j], dpLeftNew[j - 1]);
            if (first[i - 1] == second[j - 1]) {
                unsigned short betterLength = dpLeft[j - 1] + 1;
                dpLeftNew[j] = std::max(dpLeftNew[j], betterLength);
            }
        }
        delete[] dpLeft;
        dpLeft = dpLeftNew;
    }
    unsigned short* dpRight = new unsigned short[size2 + 1];
    for (int j = 0; j <= size2; ++j) {
        dpRight[j] = 0;
    }
    for (unsigned short i = size1 - 1; i >= middleFirst; --i) {
        unsigned short* dpRightNew = new unsigned short[size2 + 1];
        dpRightNew[size2] = 0;
        for (short j = static_cast<short>(size2 - 1); j >= 0; --j) {
            dpRightNew[j] = std::max(dpRight[j], dpRightNew[j + 1]);
            if (first[i] == second[j]) {
                unsigned short betterLength = dpRight[j + 1] + 1;
                dpRightNew[j] = std::max(dpRightNew[j], betterLength);
            }
        }
        delete[] dpRight;
        dpRight = dpRightNew;
    }
    unsigned short maxSum = 0;
    unsigned short middleSecond = 0;
    for (unsigned short i = 0; i <= size2; ++i) {
        if (maxSum < dpLeft[i] + dpRight[i]) {
            maxSum = dpLeft[i] + dpRight[i];
            middleSecond = i;
        }
    }
    std::string firstLeftPart;
    std::string firstRightPart;
    std::string secondLeftPart;
    std::string secondRightPart;
    for (unsigned short i = 0; i < middleFirst; ++i) {
        firstLeftPart += first[i];
    }
    for (unsigned short i = middleFirst; i < size1; ++i) {
        firstRightPart += first[i];
    }
    for (unsigned short i = 0; i < middleSecond; ++i) {
        secondLeftPart += second[i];
    }
    for (unsigned short i = middleSecond; i < size2; ++i) {
        secondRightPart += second[i];
    }
    std::string partAnswer = smartAnswer(firstLeftPart, secondLeftPart, middleFirst, middleSecond);
    partAnswer += smartAnswer(firstRightPart, secondRightPart, size1 - middleFirst, size2 - middleSecond);
    return partAnswer;
}

int main() {
    std::string first;
    std::string second;
    std::cin >> first >> second;
    unsigned short size1 = first.size();
    unsigned short size2 = second.size();
    std::cout << smartAnswer(first, second, size1, size2);
}