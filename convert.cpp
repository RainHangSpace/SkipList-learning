#include <sstream>
#include <string>
#include <stdexcept>
#include <typeinfo>

template <typename T>
T convertFromString(const std::string &str) {
    std::istringstream iss(str);
    T value;
    if (!(iss >> value)) {
        throw std::runtime_error("Conversion failed for type " + std::string(typeid(T).name()));
    }
    return value;
}

// 特化为 string
template <>
std::string convertFromString<std::string>(const std::string &str) {
    return str;
}

// 特化为 bool
template <>
bool convertFromString<bool>(const std::string &str) {
    if (str == "true" || str == "1") {
        return true;
    } else if (str == "false" || str == "0") {
        return false;
    }
    throw std::runtime_error("Conversion failed for bool");
}

// 用法示例
#include <iostream>

int main() {
    try {
        int i = convertFromString<int>("123");
        double d = convertFromString<double>("123.456");
        std::string s = convertFromString<std::string>("hello");
        bool b = convertFromString<bool>("true");

        std::cout << "int: " << i << "\ndouble: " << d << "\nstring: " << s << "\nbool: " << b << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
