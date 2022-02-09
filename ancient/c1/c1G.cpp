#include <iostream>
#include <vector>

const long long MaxMonsterHP = 1e9;
long long battle(long long mainDamage, long long  splashDamage, long long bossHP, long long monsterAmount,
                            std::vector <long long>& hp) {
    if (mainDamage == splashDamage) {
        return (bossHP + mainDamage - 1) / splashDamage;
    }
    long long left = 0;
    long long right = MaxMonsterHP;
    while (right - left > 1) {
        long long middle = (right + left) / 2;
        long long hitAmount = 0;
        long long zero = 0;
        for (int i = 0; i < monsterAmount; ++i) {
            hitAmount += (std::max((hp[i] - middle * splashDamage), zero) + mainDamage - splashDamage - 1) / (mainDamage - splashDamage);
        }
        hitAmount > middle ? left = middle : right = middle;
    }
    return right;
}
void findBoss(long long& bossHP, long long newHP) {
    if (newHP > bossHP) {
        bossHP = newHP;
    }
}
int main(){
    long long monsterAmount;
    long long mainDamage;
    long long splashDamage;
    long long bossHP = 0;
    std::cin >> monsterAmount >> mainDamage >> splashDamage;
    std::vector <long long> hp(monsterAmount);
    for (int i = 0; i < monsterAmount; ++i) {
        std::cin >> hp[i];
        findBoss(bossHP, hp[i]);
    }
    long long hitAmount = battle(mainDamage, splashDamage, bossHP, monsterAmount, hp);
    std::cout << hitAmount;
}
