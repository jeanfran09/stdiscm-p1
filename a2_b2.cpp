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

// timestamp string
string timestamp() {
    auto now = system_clock::now();
    auto itt = system_clock::to_time_t(now);
    string ts = ctime(&itt);
    ts.pop_back(); // remove newline
    return ts;
}

// Variant AII_BII
vector<int> primes_list;

void worker_divisibility_collect(int tid, int num, bool &isPrimeFlag, mutex &m) {
    for (int i = 2 + tid; i * i <= num; i += NUM_THREADS) {
        if (num % i == 0) {
            lock_guard<mutex> lock(m);
            isPrimeFlag = false;
            return;
        }
    }
}

int main() {
    loadConfig();
    cout << "Run started: " << timestamp() << endl;

    for (int n = 2; n <= MAX_NUM; n++) {
        bool isPrimeFlag = true;
        mutex m;
        vector<thread> threads;

        for (int t = 0; t < NUM_THREADS; t++) {
            threads.emplace_back(worker_divisibility_collect, t, n, ref(isPrimeFlag), ref(m));
        }
        for (auto &th : threads) th.join();

        if (isPrimeFlag) primes_list.push_back(n);
    }

    for (int p : primes_list) cout << p << " ";
    cout << endl;

    cout << "Run ended: " << timestamp() << endl;
}
