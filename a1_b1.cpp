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

// Variant AI_BI
void worker_range(int start, int end, int tid) {
    for (int i = start; i <= end; i++) {
        if (isPrime(i)) {
            lock_guard<mutex> lock(print_mutex);
            cout << "[Thread " << tid << " | " << timestamp() << "] Prime: " << i << endl;
        }
    }
}

int main() {
    loadConfig();
    cout << "Run started: " << timestamp() << endl;

    vector<thread> threads;
    int chunk = MAX_NUM / NUM_THREADS;

    for (int t = 0; t < NUM_THREADS; t++) {
        int start = t * chunk + 1;
        int end = (t == NUM_THREADS - 1) ? MAX_NUM : (t + 1) * chunk;
        threads.emplace_back(worker_range, start, end, t);
    }

    for (auto &th : threads) th.join();

    cout << "Run ended: " << timestamp() << endl;
}
