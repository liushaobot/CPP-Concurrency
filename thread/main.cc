#include <iostream>
#include <string>
#include <thread>

void hello(std::string string) {
    std::cout << "Hello, " << string << std::endl;
    std::cout << "Child thread id: " << std::this_thread::get_id() << std::endl;
}

int main(int argc, const char **argv) {
    std::cout << "Main thread id: " << std::this_thread::get_id() << std::endl;
    std::cout << "Number of supported threads: " << std::thread::hardware_concurrency() << std::endl;
    std::string string{"world!"};
    std::thread t{hello, string};
    t.join();
    
    return 0;
}