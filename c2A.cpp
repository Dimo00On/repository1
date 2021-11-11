#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

bool isFirstStringMoreSecondString(std::string firstString, std::string secondString) {
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
void pleaseSolveThisTask(const std::string& uselessString) {
    std::ifstream in("number.in");
    std::vector<std::string> numbers;
    std::string nextNumber;
    int numberAmount = 0;
    while (in >> nextNumber) {
        numbers.push_back(nextNumber);
        ++numberAmount;
    }
    std::sort(numbers.begin(), numbers.end(), isFirstStringMoreSecondString);
    std::ofstream out("number.out");
    for (int i = 0; i < numberAmount; ++i) {
        out << numbers[i];
    }
}
int main(){
    pleaseSolveThisTask("A task");
}
