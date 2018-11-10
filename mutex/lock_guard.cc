#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

std::mutex g_mutex;

void print_hello(std::string string) {
    std::lock_guard<std::mutex> lock(g_mutex);
    //g_mutex.lock();
    std::cout << "Hello, " << string << std::endl;
    //g_mutex.unlock();
}

int main(int argc, const char ** argv) {
    std::vector<std::string> strings;
    strings.push_back("World");
    strings.push_back("China");
    strings.push_back("Beijing");
    std::vector<std::thread> threads;
    for (auto& string : strings) {
        threads.push_back(std::thread(print_hello, string));
    }
    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}