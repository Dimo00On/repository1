    #include <iostream>
    #include <fstream>
    #include <vector>
    #include <string>
    #include <algorithm>
     
    namespace Big{
        int amount = 0;
    }
    bool compare(std::string firstString, std::string secondString) {
        std::string stringOne = firstString;
        firstString += secondString;
        secondString += stringOne;
        size_t i = 0;
        while (firstString[i] == secondString[i]) {
            ++i;
            if (i == firstString.size()) {
                return 0;
            }
        }
        return firstString[i] > secondString[i];
    }
    int main(){
        using namespace Big;
        std::ifstream fin("number.in");
        std::vector<std::string> numbers;
        std::string nextNumber;
        while (fin >> nextNumber) {
            numbers.push_back(nextNumber);
            ++amount;
        }
        std::sort(numbers.begin(), numbers.end(), compare);
        std::ofstream fout("number.out");
        for (int i = 0; i < amount; ++i) {
            fout << numbers[i];
        }
    }
