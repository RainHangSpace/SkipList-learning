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
#include <sstream>
#include <exception>
#include <typeinfo>

constexpr char delimiter[] = ":";
 
//class declaration for SkipList
template<typename K, typename V>
class SkipList;

//Class template to implement node
template<typename K, typename V>
class Node {
    friend class SkipList<K, V>;
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
    bool insert_element(K, V);
    void display_list();
    bool search_element(K);
    bool delete_element(K);

    void dump_file(const std::string);
    void load_file(const std::string);

    void clear(Node<K, V>*);
    size_t size();
    bool is_valid_string(const std::string& str);
    void get_key_value_from_string(const std::string& str, std::string& key, std::string& value);

    
private:
    //Maximum level of the skip list
    size_t _max_level;

    //current level of skip list
    size_t _skip_list_level;

    //head node for skiplist
    Node<K, V> *_header;

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
    :_max_level{max_level}, _skip_list_level{0}, _header{nullptr},
    _element_count{0}
{
    K k;//default key
    V v;//default value
    try {
        _header = new Node<K, V>(k, v, _max_level);
    } catch(std::bad_alloc& e) {
        std::cerr << "Bad allocation for SkipList: " << e.what() << std::endl;
        _header = nullptr;
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

template<typename K, typename V>
Node<K, V>* SkipList<K, V>::create_node(K k, V v, size_t level) {
    Node<K, V>* p = nullptr;
    try {
        p = new Node<K, V>(k, v, level);
    }
    catch(const std::bad_alloc& e) {
        std::cerr << "SkipList::create_node error! " << e.what() << '\n';
        p = nullptr;
        throw;
    }
    return p;
}

//查找
template<typename K, typename V>
bool SkipList<K, V>::search_element(K target) {
    Node<K, V>* current = _header;
    //注意：用size_t进行索引的时候 不能以大于等于零作判断条件
    for (size_t i = _skip_list_level + 1; i > 0; --i) {
        while(current->_forward[i - 1] && current->_forward[i - 1]->get_key() < target) {
            current = current->_forward[i - 1];
        }
    }
    current = current->_forward[0];//一定会下沉到最底层
    if (!current || current->get_key() > target)
        return false;
    return true;
}
//插入
template<typename K, typename V>
bool SkipList<K, V>::insert_element(K k, V v) {
    
    //用current去查找需要插入的位置
    Node<K, V>* current = _header;
    // current = current->_forward[0];
    // current = _header;
    // std::cout << "current:" << _header << '\n';
    //用数组记录插入节点时，需要更新的前驱结点
    Node<K, V>* pre_node_to_update[_max_level + 1];
    for (size_t i = 0; i < _max_level + 1; ++i) {
        pre_node_to_update[i] = nullptr;
    }

    //注意：用size_t进行索引的时候 不能以大于等于零作判断条件
    for (size_t i = _skip_list_level + 1; i > 0; --i) {
        // std::cout << "i: " << i << '\n';
        while (current->_forward[i - 1] && current->_forward[i - 1]->get_key() < k) {
            // std::cout << i << " ";
            current = current->_forward[i - 1];
        }
        // std::cout << "i: " << i << '\n';
        //当退出上面的while时，就说明找到了要插入的位置，即current的后面，所以要存储到数组中，等待更新
        pre_node_to_update[i - 1] = current;
        // std::cout << "b\n";        
    }
    // std::cout << "c\n";
    current = current->_forward[0];
    // std::cout << "d: " << current << '\n';
    if (current && current->get_key() == k)
        return false;//如果不用插入 返回false
    
    size_t level = get_random_level();//随机生成level
    // std::cout << "d: " << level << '\n';    
    if (level > _skip_list_level) {
        for (size_t i = _skip_list_level + 1; i <= level; ++i) {
            pre_node_to_update[i] = _header;
        }
        //记得更新_skip_list_level
        _skip_list_level = level;
    }
    // std::cout << "_skip_list_level = " << _skip_list_level << '\n';
    Node<K, V>* node_p = create_node(k, v, level);
    for (size_t i = 0; i <= level; ++i) {
        node_p->_forward[i] = pre_node_to_update[i]->_forward[i];
        pre_node_to_update[i]->_forward[i] = node_p;
    }
    //记得更新元素数量
    ++_element_count;
    return true;
}

//display
template<typename K, typename V>
void SkipList<K, V>::display_list() {
    Node<K, V>* current = nullptr;
    //注意：用size_t进行索引的时候 不能以大于等于零作判断条件
    for (size_t i = _skip_list_level + 1; i > 0; --i) {
        std::cout << "level" << i - 1 << ": ";
        current = _header->_forward[i - 1];
        while (current) {
            std::cout << "(" << current->get_key() << ", " << current->get_value() << ") ";
            current = current->_forward[i - 1];
        }
        std::cout << '\n';
    }
    std::cout << std::endl;
}

// 删除
template<typename K, typename V> 
bool SkipList<K, V>::delete_element(K target) {
    Node<K, V>* current = _header;
    Node<K, V>* pre_node_to_update[_skip_list_level + 1];

    for (size_t i = _skip_list_level + 1; i > 0; --i) {
        while (current->_forward[i - 1] && current->_forward[i - 1]->get_key() < target) {
            current = current->_forward[i - 1];
        }
        pre_node_to_update[i - 1] = current;
    }

    current = current->_forward[0];
    if (current && current->get_key() == target) {
        size_t cur_level = current->_nodeLevel;
        for (size_t i = 0; i <= cur_level; ++i) {
            pre_node_to_update[i]->_forward[i] = current->_forward[i];
        }
        delete current;
        --_element_count;
        while(_skip_list_level > 0 && _header->_forward[_skip_list_level] == nullptr) {
            --_skip_list_level;
        }
        return true;
    }
    return false;
}

//保存数据
template <typename K, typename V>
void SkipList<K, V>::dump_file(const std::string relative_write_file_path) {
    _file_writer.open(relative_write_file_path); // 打开文件
    Node<K, V>* node = _header->_forward[0]; // 从头节点开始遍历

    while (node != nullptr) {
        _file_writer << node->get_key() << ":" << node->get_value() << ";\n"; // 写入键值对
        node = node->_forward[0]; // 移动到下一个节点
    }

    _file_writer.flush(); // 刷新缓冲区，确保数据完全写入
    _file_writer.close(); // 关闭文件
}

template <typename K, typename V>
bool SkipList<K, V>::is_valid_string(const std::string& str) {
    return !str.empty() && str.find(delimiter) != std::string::npos;
}

template <typename K, typename V>
void SkipList<K, V>::get_key_value_from_string(const std::string& str, std::string& key, std::string& value) {
    if (!is_valid_string(str)) {
        return;
    }
    key = str.substr(0, str.find(delimiter));
    value = str.substr(str.find(delimiter) + 1);
}

//读取数据
template <typename K, typename V>
void SkipList<K, V>::load_file(const std::string relative_read_file_path) {
    _file_reader.open(relative_read_file_path);
    std::string line;
    std::string key;
    std::string value;

    while (getline(_file_reader, line)) {
        get_key_value_from_string(line, key, value);
        if (key.empty() || value.empty()) {
            continue;
        }
        
        std::istringstream iss_key(key);
        std::istringstream iss_value(value);
        K key_converted;
        V value_converted;
        if (!(iss_key >> key_converted))
            throw std::runtime_error("Unsupported type " + std::string(typeid(K).name()));
            // throw std::runtime_error("Conversion failed for type " + std::string(typeid(T).name()));
        if (!(iss_value >> value_converted))
            throw std::runtime_error("Unsupported type " + std::string(typeid(V).name()));
        insert_element(key_converted, value_converted);
        // std::cout << "key:" << key << "value:" << value << std::endl;
    }

    // delete key;
    // delete value;
    _file_reader.close();
}
