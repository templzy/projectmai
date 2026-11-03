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

// Функция для имитации передачи одного бита
int transmit_bit(int bit, double error_prob, int repeats, mt19937& gen) {
    if (repeats == 1) {
        bernoulli_distribution error(error_prob);
        return error(gen) ? 1 - bit : bit;
    }
    
    // Для 3 и 5 повторений
    bernoulli_distribution error(error_prob);
    int ones_count = 0;
    for (int i = 0; i < repeats; ++i) {
        int received = error(gen) ? 1 - bit : bit;
        if (received == 1) ones_count++;
    }
    
    // Мажоритарное голосование
    if (repeats == 3) return (ones_count >= 2) ? 1 : 0;
    if (repeats == 5) return (ones_count >= 3) ? 1 : 0;
    return 0;
}

int main() {
    mt19937 gen(42); // Фиксированный сид для воспроизводимости
    vector<double> probs = {0.005, 0.02, 0.05};
    int experiments = 10000;
    
    // Вывод заголовка таблицы
    cout « "Вероятность | Метод | % без ошибок | Ср. ошибок | Передано бит\n";
    cout « "----------------------------------------------------------------\n";

    for (double p : probs) {
        for (int repeats : {1, 3, 5}) {
            int clean_messages = 0;
            long long total_bit_errors = 0;
            long long total_transmitted = 0;

            for (int exp = 0; exp < experiments; ++exp) {
                // 1. Генерируем исходное сообщение
                vector<int> original = generate_message(gen);
                vector<int> received(128);
                bool has_error = false;
                int bit_errors = 0;

                // 2. Передаем 128 бит с учетом повторов
                for (int i = 0; i < 128; ++i) {
                    received[i] = transmit_bit(original[i], p, repeats, gen);
                    if (received[i] != original[i]) {
                        has_error = true;
                        bit_errors++;
                    }
                }

                if (!has_error) clean_messages++;
                total_bit_errors += bit_errors;
                total_transmitted += 128 * repeats;
            }

            // Считаем статистику
            double clean_percent = (double)clean_messages / experiments * 100.0;
            double avg_errors = (double)total_bit_errors / experiments;
            double avg_transmitted = (double)total_transmitted / experiments;

            // Вывод строки
            cout « fixed « setprecision(3);
            cout « setw(10) « p * 100 « "% | "
                 « setw(5) « repeats « "x | "
                 « setw(11) « clean_percent « "% | "
                 « setw(10) « avg_errors « " | "
                 « setw(13) « avg_transmitted « "\n";
        }
    }
    return 0;
}
