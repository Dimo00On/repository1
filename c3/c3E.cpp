#include <iostream>
#include <vector>
#include <algorithm>

struct CommandType{
    bool type;
    long long value;
    int number;
    int position;
    int insertNumber;
    CommandType(bool newType, long long newValue, int newNumber, int newPosition, int newInsertNumber) {
        type = newType;
        value = newValue;
        number = newNumber;
        position = newPosition;
        insertNumber = newInsertNumber;
    }
};
struct Segment{
    int left = 0;
    int right = 0;
    long long sum = 0;
};
int findRight(long long value, std::vector<CommandType>& elements) {
    int left = -1;
    int right = static_cast<int>(elements.size());
    while (right - left > 1) {
        int middle = (left + right) / 2;
        if (elements[middle].value > value) {
            right = middle;
        } else {
            left = middle;
        }
    }
    return left;
}
void foundSum(int left, int right, int index, long long& sum, std::vector<Segment>& tree){
    if (tree[index].left == left && tree[index].right == right) {
        sum += tree[index].sum;
        return;
    }
    int middle = (tree[index].left + tree[index].right) / 2;
    if (middle >= left) {
        foundSum(left, std::min(middle, right), 2 * index, sum, tree);
    }
    if (middle < right) {
        foundSum(std::max(left, middle + 1), right, 2 * index + 1, sum, tree);
    }
}
void build(int index, std::vector<Segment>& tree, int perfectSize) {
    if (index >= perfectSize) {
        return;
    }
    build(2 * index, tree, perfectSize);
    build(2 * index + 1, tree, perfectSize);
    tree[index].left = tree[2 * index].left;
    tree[index].right = tree[2 * index + 1].right;
}
void update(int index, std::vector<Segment>& tree) {
    if (index == 0) {
        return;
    }
    tree[index].sum = tree[2 * index].sum + tree[2 * index + 1].sum;
    update(index / 2, tree);
}
bool comparator(CommandType element1, CommandType element2) {
    if (element1.value == element2.value) {
        return element1.insertNumber < element2.insertNumber;
    }
    return element1.value < element2.value;
}
void fillArrays(char operation, long long value, int& operationNumber, int step, std::vector<CommandType>& elements,
                std::vector<CommandType>& command) {
    CommandType newCommand(operation == '?', value, operationNumber, step - operationNumber, step);
    if (operation == '?') {
        ++operationNumber;
    } else {
        elements.push_back(newCommand);
    }
    command.push_back(newCommand);
}
int findPerfectSize(int badSize) {
    int copy = badSize - 1;
    int perfectSize = 1;
    while (copy > 0) {
        copy /= 2;
        perfectSize *= 2;
    }
    return perfectSize;
}
void fillTree(int badSize, int perfectSize, std::vector<int>& indexes, std::vector<Segment>& tree,
              std::vector<CommandType>& elements) {
    for (int i = 0; i < badSize; ++i) {
        indexes[elements[i].position] = i;
        tree[i + perfectSize].left = tree[i + perfectSize].right = i;
    }
    for (int i = badSize; i < perfectSize; ++i) {
        tree[i + perfectSize].left = tree[i + perfectSize].right = i;
    }
}
void allLogic(int commandAmount,int perfectSize, std::vector<CommandType>& command, std::vector<CommandType>& elements,
              std::vector<int>& indexes, std::vector<Segment>& tree, std::vector<long long>& answer) {
    for (int i = 0; i < commandAmount; ++i) {
        if (command[i].type) {
            long long sum = 0;
            int right = findRight(command[i].value, elements);
            if (right == -1) {
                answer[command[i].number] = 0;
                continue;
            }
            foundSum(0, right, 1, sum, tree);
            answer[command[i].number] = sum;
        } else {
            int index = indexes[command[i].position] + perfectSize;
            tree[index].sum = command[i].value;
            update(index / 2, tree);
        }
    }
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int commandAmount;
    std::cin >> commandAmount;
    std::vector<CommandType> command;
    std::vector<CommandType> elements;
    int operationNumber = 0;
    for (int i = 0; i < commandAmount; ++i) {
        char operation;
        long long value;
        std::cin >> operation >> value;
        fillArrays(operation, value, operationNumber, i, elements, command);
    }
    int badSize = commandAmount - operationNumber;
    int perfectSize = findPerfectSize(badSize);
    std::vector<int> indexes(commandAmount - operationNumber);
    std::sort(elements.begin(), elements.end(), comparator);
    std::vector<Segment> tree(2 * perfectSize);
    fillTree(badSize, perfectSize, indexes, tree, elements);
    build(1, tree, perfectSize);
    std::vector<long long> answer(operationNumber);
    allLogic(commandAmount, perfectSize, command, elements, indexes, tree, answer);
    for (int i = 0; i < operationNumber; ++i) {
        std::cout << answer[i] << '\n';
    }
}
