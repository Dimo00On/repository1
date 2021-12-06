#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <iterator>

void add(unsigned long long value, int index, std::vector<std::set<unsigned long long>>& sets,
         std::map<unsigned long long, std::set<int>>& inclusion) {
    sets[index].insert(value);
    if (inclusion.find(value) == inclusion.end()) {
        std::pair<unsigned long long, std::set<int>> temp;
        temp.second.insert(index);
        temp.first = value;
        inclusion.insert(temp);
    } else {
        inclusion[value].insert(index);
    }
}
void clear(int index, std::vector<std::set<unsigned long long>>& sets, std::map<unsigned long long, std::set<int>>& inclusion) {
    for (auto j = sets[index].begin(); j != sets[index].end(); ++j) {
        inclusion[*j].erase(index);
    }
    sets[index].clear();
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    unsigned long long maxValue;
    int maxSet;
    int commandAmount;
    std::cin >> maxValue >> maxSet >> commandAmount;
    std::vector<std::set<unsigned long long>> sets(maxSet + 1);
    std::map<unsigned long long, std::set<int>> inclusion;
    for (int i =0; i < commandAmount; ++i) {
        unsigned long long value;
        int index;
        std::string command;
        std::cin >> command;
        if (command[0] == 'A') {
            std::cin >> value >> index;
            add(value, index, sets, inclusion);
        }
        if (command[0] == 'D') {
            std::cin >> value >> index;
            sets[index].erase(value);
            inclusion[value].erase(index);
        }
        if (command[0] == 'C') {
            std::cin >> index;
            clear(index, sets, inclusion);
        }
        if (command == "LISTSET") {
            std::cin >> index;
            if (sets[index].begin() == sets[index].end()) {
                std::cout << -1 << '\n';
            } else {
                std::copy(sets[index].begin(), sets[index].end(), std::ostream_iterator<unsigned long long>(std::cout, " "));
                std::cout << '\n';
            }
        }
        if (command == "LISTSETSOF") {
            std::cin >> value;
            if (inclusion.find(value) == inclusion.end() || inclusion[value].begin() == inclusion[value].end()) {
                std::cout << -1 << '\n';
            } else {
                std::copy(inclusion[value].begin(), inclusion[value].end(), std::ostream_iterator<unsigned long long>(std::cout, " "));
                std::cout << '\n';
            }
        }
    }
}
