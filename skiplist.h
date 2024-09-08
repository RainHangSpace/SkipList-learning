/*
>File Name:     main.cpp
>Author:        rAiN
>Mail:          1017543088@qq.com
>Created Time:  20240908
>Description:    
*/

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>


//Class template to implement node
template<typename K, typename V>
class Node {
public:
    Node() {}

    Node(K, V, size_t);

    ~Node();

    K get_key() const;

    V get_value() const;

    void set_value(V); 

private:
    K _key;
    V _value;
    size_t _nodeLevel;
    //Linear array to hold pointers to next node of different level
    Node<K, V> **_forward;
};

template<typename K, typename V>
Node<K, V>::Node(const K k, const V v, size_t level)
    :_key{k}, _value{v}, _nodeLevel{level}, _forward{nullptr}    
{
    try {
        _forward = new Node<K, V>*[level + 1];
    } catch(std::bad_alloc& e) {
        std::cerr << "Memory allocation failed for _forward arrey: " << e.what() << std::endl;
        _forward = nullptr;
        throw;
    }
    // memset(_forward, 0, sizeof(Node<K, V>*)*(level+1));
    for (int i = 0; i <= level; ++i) {
        _forward[i] = nullptr;
    }
}

template<typename K, typename V>
Node<K, V>::~Node() {
    delete []_forward;
}

template<typename K, typename V>
K Node<K, V>::get_key() const {
    return _key;
}

template<typename K, typename V>
V Node<K, V>::get_value() const {
    return _value;
}

template<typename K, typename V>
void Node<K, V>::set_value(V val) {
    _value = val;
    return;
}


//Class template for skip list
template <typename K, typename V>
class SkipList {
public:
    SkipList(size_t);
    ~SkipList();
    size_t get_random_level();
    Node<K, V>* create_node(K, V, size_t);
    int insert_element(K, V);
    void display_list();
    bool search_element(K);
    void delete_element(K);
    void dump_file();
    void load_file();
    void clear(Node<K, V>*);
    size_t size();
    
private:
    //Maximum level of the skip list
    size_t _max_level;

    //current level of skip list
    size_t _skip_list_level;

    //head node for skiplist
    Node<K, V> *_head;

    //number of elements
    size_t _element_count;

    //file writer
    std::ofstream _file_writer;

    //file reader
    std::ifstream _file_reader;
public:

};

template<typename K, typename V>
SkipList<K, V>::SkipList(size_t max_level) 
    :_max_level{max_level}, _skip_list_level{0}, _head{nullptr},
    _element_count{0}
{
    K k;
    V v;
    try {
        _head = new Node<K, V>(k, v, _max_level);
    } catch(std::bad_alloc& e) {
        std::cerr << "Bad allocation for SkipList: " << e.what() << std::endl;
        _head = nullptr;
        throw;
    }
}
template<typename K, typename V>
SkipList<K, V>::~SkipList() {
    ;
}
template<typename K, typename V>
size_t SkipList<K, V>::get_random_level() {
    size_t k = 1;

    while (rand() % 2)
        k++;
    
    k = (k < _max_level) ? k : _max_level;

    return k;
}




