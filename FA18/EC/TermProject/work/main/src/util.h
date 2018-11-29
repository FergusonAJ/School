#pragma once

#include <bitset>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <locale>

std::vector<unsigned char> BitstringToVec(std::vector<bool> bs, size_t offset = 0){
    std::vector<unsigned char> vec;
    vec.resize(bs.size() - offset);
    for(size_t i = 0; i < vec.size(); i++){
        vec[i] = bs[i + offset] ? 1 : 0;
    }
    return vec;
}

std::string GetTimestamp(){
    std::cout << "Testing..." << std::endl;
    std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char buff[100];
    std::strftime(buff, sizeof(buff), "%Y_%m_%d_%M_%S", std::localtime(&t));
    return std::string(buff);
}
