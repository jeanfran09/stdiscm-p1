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

// Variant AII_BI
vector<int> results;
mutex results_mutex;

void worker_range_collect(int start, int end) {
    vector<int> local;
    for (int i = start; i <= end; i++) {
        if (isPrime(i)) local.push_back(i);
    }
    lock_guard<mutex> lock(results_mutex);
    results.insert(results.end(), local.begin(), local.end());
}

int main() {
    loadConfig();
    cout << "Run started: " << timestamp() << endl;

    vector<thread> threads;
    int chunk = MAX_NUM / NUM_THREADS;

    for (int t = 0; t < NUM_THREADS; t++) {
        int start = t * chunk + 1;
        int end = (t == NUM_THREADS - 1) ? MAX_NUM : (t + 1) * chunk;
        threads.emplace_back(worker_range_collect, start, end);
    }

    for (auto &th : threads) th.join();

    for (int p : results) cout << p << " ";
    cout << endl;

    cout << "Run ended: " << timestamp() << endl;
}
