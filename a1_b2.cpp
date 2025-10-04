#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <sstream>

using namespace std;
using namespace std::chrono;

int NUM_THREADS;
int MAX_NUM;

mutex print_mutex;

// read config
void loadConfig() {
    ifstream in("config.txt");
    char key;
    int value;
    while (in >> key >> value) {
        if (key == 'x') {
            NUM_THREADS = value;
        } else if (key == 'y') {
            MAX_NUM = value;
        }
    }
}

// simple primality check
bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

// timestamp string
string timestamp() {
    auto now = system_clock::now();
    auto itt = system_clock::to_time_t(now);
    string ts = ctime(&itt);
    ts.pop_back(); // remove newline
    return ts;
}

// Variant AI_BII
void worker_divisibility(int tid, int maxNum, mutex &m) {
    for (int i = 2 + tid; i <= maxNum; i += NUM_THREADS) {
        if (isPrime(i)) {
            lock_guard<mutex> lock(m);
            cout << "[Thread " << tid << " | " << timestamp() << "] Prime: " << i << endl;
        }
    }
}

int main() {
    loadConfig();
    cout << "Run started: " << timestamp() << endl;

    vector<thread> threads;

    for (int t = 0; t < NUM_THREADS; t++) {
        threads.emplace_back(worker_divisibility, t, MAX_NUM, ref(print_mutex));
    }
    for (auto &th : threads) th.join();

    cout << "Run ended: " << timestamp() << endl;
}