#include <iostream>
#include <vector>
#include <random>
#include <iomanip>

using namespace std;

// Функция для генерации случайного сообщения из 128 бит
vector<int> generate_message(mt19937& gen) {
    vector<int> msg(128);
    uniform_int_distribution<> dis(0, 1);
    for (int i = 0; i < 128; ++i) {
        msg[i] = dis(gen);
    }
    return msg;
}
