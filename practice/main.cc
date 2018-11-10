#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "thread_safe_queue.h"

std::mutex m_mutex;

void producer(ThreadSafeQueue<int>& product, std::vector<int>& nums, int num) {
    m_mutex.lock();
    std::cout << "Producer Thread " << std::this_thread::get_id() << " starts working..." << std::endl;
    m_mutex.unlock();
    for (int i = 0; i < num; i++) {
        product.push(i);
        nums.push_back(product.back());
    }
    m_mutex.lock();
    std::cout << "Producer Thread " << std::this_thread::get_id() << " finished..." << std::endl;
    m_mutex.unlock();
}

void consumer(ThreadSafeQueue<int>& product, std::vector<int>& nums, int num) {
    m_mutex.lock();
    std::cout << "Consumer Thread " << std::this_thread::get_id() << " starts working..." << std::endl;
    m_mutex.unlock();
    while (num != 0) {
        int front;
        product.pop(front);
        nums.push_back(front);
        --num;
    }
    m_mutex.lock();
    std::cout << "Consumer Thread " << std::this_thread::get_id() << " finished..." << std::endl;
    m_mutex.unlock();
}

int main(int argc, char **argv) {
    m_mutex.lock();
    std::cout << "Main Thread " << std::this_thread::get_id() << " starts working..." << std::endl;
    m_mutex.unlock();

    ThreadSafeQueue<int> product;
    std::vector<int> fronts;
    std::vector<int> backs;
    int num = 100;
    std::thread produce(producer, std::ref(product), std::ref(backs), num);
    std::thread consume(consumer, std::ref(product), std::ref(fronts), num);
    produce.join();
    consume.join();

    m_mutex.lock();
    std::cout << "Main Thread " << std::this_thread::get_id() << " finished..." << std::endl;
    m_mutex.unlock();
    for (auto& front : fronts) {
        std::cout << front << " ";
    }
    std::cout << std::endl;
    for (auto& back : backs) {
        std::cout << back << " ";
    }
    std::cout << std::endl;

    return 0;
}