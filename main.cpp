/*
>File Name:     main.cpp
>Author:        rAiN
>Mail:          1017543088@qq.com
>Created Time:  20240908
>Description:    
*/

#include <iostream>
#include "skiplist.h"
#include <string>
#include <ctime>

constexpr char WRITE_FILE[] = "./data/writeDB";
int main() {
    std::srand(std::time(0)); // 设置随机种子
    //测试1：
    // SkipList<int, int> skipList1(6);
    // SkipList<std::string, int> skipList2(6);
    // Node<int, int> node1(1, 1, 0);
    // Node<int, std::string> node2(2, "b", 0);
    // Node<int, int>* node1_p = &node1;
    // Node<int, std::string>* node2_p = &node2; 
    // Node<std::string, int>* node3_p = skipList2.create_node("zhangyuhan", 363, 0);
    // std::cout << node1.get_key() << ' ' << node1_p->get_value() << '\n';
    // std::cout << node2.get_key() << ' ' << node2_p->get_value() << '\n';
    // std::cout << node3_p->get_key() << ' ' << node3_p->get_value() << '\n';
    
    
    //测试2：
    // SkipList<std::string, int> SkipList1(7);
    // SkipList1.insert_element("zhangyuhan", 363);
    // SkipList1.display_list();
    // std::cout << '\n';
    // SkipList1.insert_element("zhangyuhan", 363);
    // SkipList1.insert_element("wangyumeng", 636);
    // SkipList1.display_list();

    // SkipList1.delete_element("zhangyuhan");
    // SkipList1.display_list();


    //测试3：
    SkipList<std::string, int> SkipList1(7);
    SkipList1.insert_element("heishenhua", 268);
    SkipList1.insert_element("wukong", 328);
    SkipList1.insert_element("zhubajie", 188);
    SkipList1.insert_element("wuneng", 648);
    SkipList1.insert_element("guangzhi", 777);
    SkipList1.insert_element("jinchanzi", 888);
    SkipList1.insert_element("zhangyuhan", 363);
    SkipList1.insert_element("wangyumeng", 348);

    //保存文件
    SkipList1.dump_file(WRITE_FILE);

    //加载文件
    SkipList<std::string, int> SkipList2(6);
    SkipList2.load_file(WRITE_FILE);
    SkipList2.display_list();

    // if (SkipList1.search_element("wangyumeng"))
    //     std::cout << "Search Success";
    // else 
    //     std::cout << "Search Failed";

}