#include <iostream>
#include <chrono>//高精度时间测量
#include <cstdlib>
#include <pthread.h>
#include <time.h>
#include "./skiplist.h"

const size_t NUM_THREADS = 3;
const size_t TEST_COUNT = 100000;

SkipList<int, std::string> skipList(18);

//线程函数：插入元素
void *thread_insert_element(void* threadid) {
    long tid = *static_cast<long*>(threadid);
    std::cout << "threadid_insert: " << tid << std::endl;
    int tmp = TEST_COUNT / NUM_THREADS;

    for (int count = 0; count < tmp; ++count) {
        skipList.insert_element(rand() % TEST_COUNT, "a");
    }
    pthread_exit(NULL);
    return nullptr;
}
//线程函数：检索元素
void *thread_search_element(void* threadid) {
    long tid = *static_cast<long*>(threadid);
    std::cout << "threadid_search: " << tid << std::endl;
    int tmp = TEST_COUNT / NUM_THREADS;
    for (int count = 0; count < tmp; ++count) {
        skipList.search_element(rand() % TEST_COUNT);
    }
    pthread_exit(NULL);
    return nullptr;
}
//main
int main() {
    srand(time(0));
    
    pthread_t threads[NUM_THREADS];
    size_t i;

    auto start = std::chrono::high_resolution_clock::now();

    //创建插入元素的线程
    for (i = 0; i < NUM_THREADS; ++i) {
        std::cout << "main() : creating thread, " << i << " for insert element" << std::endl;
        if (pthread_create(&threads[i], 0, thread_insert_element, (void*)&i)) {
            std::cout << "thread creating error for insert element" << std::endl;
            exit(-1);
        }
    }

    //等待所有插入线程完成
    void * retval;//用于接收插入线程的返回值，当然此例子没有返回值
    for (i = 0; i < NUM_THREADS; ++i) {
        if (pthread_join(threads[i], &retval)) {
            perror("thread joining error for searching");
            exit(3);
        }
    }
    

    //创建查找元素的线程
    auto start2 = std::chrono::high_resolution_clock::now();
    for (i = 0; i < NUM_THREADS; ++i) {
        std::cout << "main() : creating thread " << i << " for search element" <<std::endl;
        if (pthread_create(&threads[i], NULL, thread_search_element, (void*)&i)) {
            std::cout << "thread creating error for searching element" << std::endl;
            exit(-1);
        }
    }
    //等待所有插入线程完成
    for (i = 0; i < NUM_THREADS; ++i) {
        if (pthread_join(threads[i], NULL)) {
            perror("thread joining error for searching");
            exit(3);
        }
    }
    
    //生成存储文件
    auto start3 = std::chrono::high_resolution_clock::now();
    skipList.dump_file("./data/stress_test.txt");
    auto finish3 = std::chrono::high_resolution_clock::now();


    //计算时间
    auto finish = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
    std::cout << "insert elapsed:" << elapsed.count() << "ms" << std::endl; // 输出插入操作耗时
    auto finish2 = std::chrono::high_resolution_clock::now();
    auto elapsed2_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish2 - start2);
    std::cout << "search elapsed:" << elapsed2_ms.count() << "ms" << std::endl; // 输出插入操作耗时
    std::chrono::duration<double> elapsed3 = finish3 - start3;
    auto elapsed3_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed3);
    std::cout << "dump elapsed:" << elapsed3_ms.count() << "ms" << std::endl; // 输出存储操作耗时

    return 0;
}