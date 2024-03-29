#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

bool comparator(std::string firstString, std::string secondString) {
    std::string temp = firstString;
    firstString += secondString;
    secondString += temp;
    size_t i = 0;
    while (firstString[i] == secondString[i]) {
        ++i;
        if (i == firstString.size()) {
            return false;
        }
    }
    return firstString[i] > secondString[i];
}
int main(){
    std::ifstream in("number.in");
    std::vector<std::string> numbers;
    std::string nextNumber;
    int numberAmount = 0;
    while (in >> nextNumber) {
        numbers.push_back(nextNumber);
        ++numberAmount;
    }
    std::sort(numbers.begin(), numbers.end(), comparator);
    std::ofstream out("number.out");
    for (int i = 0; i < numberAmount; ++i) {
        out << numbers[i];
    }
}
