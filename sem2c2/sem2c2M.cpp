#include <iostream>
#include <vector>

bool isThereBit(int mask, int bit) {
    return static_cast<bool>(mask & (1 << bit));
}

void findAnswer(long long& answer, std::vector<std::vector<bool>>& friends, int amount) {
    int left = amount / 2;
    int right = amount - left;
    std::vector<int> neighbors(left, 0);
    for (int i = 0; i < left; ++i) {
        for (int j = 0; j < right; ++j) {
            neighbors[i] += (1 << j) * static_cast<int>(friends[i][left + j]);
        }
    }
    std::vector<int> allNeighbors(1 << left, 0);
    allNeighbors[0] = (1 << right) - 1;
    std::vector<int> numberBit(1 << right, 0);
    for (int i = 0; i < right; ++i) {
        numberBit[1 << i] = i;
    }
    std::vector<bool> clique(1 << left, false);
    clique[0] = true;
    for (int i = 0; i < left; ++i) {
        clique[1 << i] = true;
    }
    for (int mask = 1; mask < (1 << left); ++mask) {
        int bit1 = numberBit[mask - (mask & (mask - 1))];
        allNeighbors[mask] = neighbors[bit1] & allNeighbors[mask - (1 << bit1)];
        if (numberBit[mask] == 0 && mask != 1) {
            int newMask = mask - (1 << bit1);
            int bit2 = numberBit[newMask - (newMask & (newMask - 1))];
            if ((newMask - (1 << bit2)) >= 0) {
                clique[mask] = clique[mask - (1 << bit1)] && clique[mask - (1 << bit2)]
                                && friends[bit1][bit2];
            }
        }
    }
    std::vector<int> amountCliques(1 << right, 0);
    amountCliques[0] = 1;
    for (int i = 0; i < right; ++i) {
        amountCliques[1 << i] = 1;
    }
    for (int mask = 1 ; mask < (1 << right); ++mask) {
        int bit1 = numberBit[mask - (mask & (mask - 1))];
        if (numberBit[mask] == 0 && mask != 1) {
            int newMask = mask - (1 << bit1);
            int bit2 = numberBit[newMask - (newMask & (newMask - 1))];
            amountCliques[mask] = static_cast<int>(amountCliques[mask - (1 << bit1)]
                    && amountCliques[mask - (1 << bit2)] && friends[left + bit1][left + bit2]);
        }
    }
    for (int i = 0; i < right; ++i) {
        for (int mask = 0; mask < (1 << right); ++mask) {
            if (!isThereBit(mask, i)) {
                amountCliques[mask + (1 << i)] += amountCliques[mask];
            }
        }
    }
    for (int mask = 0; mask < (1 << left); ++mask) {
        if (clique[mask]) {
            answer += static_cast<long long>(amountCliques[allNeighbors[mask]]);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int amount;
    std::cin >> amount;
    std::vector<std::vector<bool>> friends(amount);
    for (int i = 0; i < amount; ++i) {
        friends[i].resize(amount);
        for (int j =0; j < amount; ++j) {
            char value;
            std::cin >> value;
            friends[i][j] = (value == '1');
        }
    }
    long long answer = 0;
    findAnswer(answer, friends, amount);
    std::cout << answer;
}
