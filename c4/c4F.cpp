#include <iostream>
#include <set>
#include <vector>
#include <map>

const int Infinity = 1e9;
struct carTime{
    std::vector<int> time;
    int now = 0;
};
void allLogic(int wishesAmount, int& answer, std::vector<carTime>& cars, int floorSize, std::map<int, int>& floor,
              std::map<int,int>& inverseFloor, std::vector<int>& wishes) {
    for (int i = 0; i < wishesAmount; ++i) {
        int number = wishes[i];
        if (!floor.count(number)) {
            ++answer;
            floor[number] = i;
            inverseFloor[i] = number;
            if (inverseFloor.size() > floorSize) {
                auto iterator = inverseFloor.end();
                --iterator;
                floor.erase(iterator->second);
                inverseFloor.erase(iterator->first);
            }
        }
        ++cars[number].now;
        int value = (cars[number].now == cars[number].time.size() ? Infinity : cars[number].time[cars[number].now]);
        inverseFloor.erase(cars[number].time[cars[number].now - 1]);
        inverseFloor[value] = number;
        floor[number] = value;
    }
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int carNumber, floorSize, wishesAmount;
    std::cin >> carNumber >> floorSize >> wishesAmount;
    std::vector<carTime> cars(carNumber + 1);
    std::vector<int> wishes;
    for (int i = 0; i < wishesAmount; ++i) {
        int number;
        std::cin >> number;
        wishes.push_back(number);
        cars[number].time.push_back(i);
    }
    int answer = 0;
    std::map<int, int> floor;
    std::map<int,int> inverseFloor;
    allLogic(wishesAmount, answer, cars, floorSize, floor, inverseFloor, wishes);
    std::cout << answer << '\n';
}
