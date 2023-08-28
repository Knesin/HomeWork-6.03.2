#include <iostream>
#include <vector>
#include <chrono>
#include <future>
#include <execution>
#include <thread>

void print_vec(const std::vector<int>& vec) { //печать вектора
    for (const auto& v : vec) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

void func(int &i) {
    i = i + 10000000;
}

void for_each(std::vector<int>::iterator start, std::vector<int>::iterator end, void (*fu)(int&)) {
    auto curr_size = std::distance(start, end);
    int proc = std::thread::hardware_concurrency();
    if (curr_size < 10000){
        for (; start != end; ++start) {
            fu(*start);
        }
        return;
    }
    auto mid = start;
    std::advance(mid, curr_size / 2);
    std::async(std::launch::async, for_each, start, mid, fu);
    //std::async(for_each, mid, end, fu);
    for_each (mid, end, fu);
}
//-------------------------Реализация на потоках---------------------------------
void for_each_2(std::vector<int>::iterator start, std::vector<int>::iterator end, void (*fu)(int&)) {
    auto curr_size = std::distance(start, end);
    int proc = std::thread::hardware_concurrency();
    std::vector<std::thread> trv(proc);
    auto f = [](std::vector<int>::iterator start, std::vector<int>::iterator end, void (*fu)(int&))
            { for (; start != end; ++start) {
                    fu(*start);
            } };
    for(int i = 0; i < proc - 1; ++i){
        trv[i] = std::move(std::thread(f, start + (i*curr_size/proc) ,start + ((i+1) * curr_size / proc), fu));
    }
    trv[proc-1] = std::move(std::thread(f, start + ((proc-1) * curr_size / proc), end, fu));
    for (int k = 0; k < proc; k++) {
        trv[k].join();
    }
}

int main() {
    const int vector_length = 20000000;// Длина вектора
    std::vector<int> vec(vector_length);
    for (auto& i : vec) {
        i = std::rand() % vector_length;
    }
    //print_vec(vec);
    //auto func = [](int& i) {i += 20; };
    auto time_start = std::chrono::high_resolution_clock::now();
    for_each(vec.begin(), vec.end(), func);
    auto time_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> t = (time_end - time_start);
    std::cout << "Time async: " << t.count() << std::endl;

    time_start = std::chrono::high_resolution_clock::now();
    for_each_2(vec.begin(), vec.end(), func);
    time_end = std::chrono::high_resolution_clock::now();
    t = (time_end - time_start);
    std::cout << "Time thread: " << t.count() << std::endl;

    time_start = std::chrono::high_resolution_clock::now();
    std::for_each(std::execution::par,vec.begin(), vec.end(), func);
    time_end = std::chrono::high_resolution_clock::now();
    t = (time_end - time_start);
    std::cout << "Time std: " << t.count() << std::endl;
    //print_vec(vec);
}