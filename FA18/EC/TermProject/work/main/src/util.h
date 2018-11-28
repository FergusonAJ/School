#pragma once

#include <bitset>
#include <vector>

std::vector<unsigned char> BitstringToVec(std::vector<bool> bs, size_t offset = 0){
    std::vector<unsigned char> vec;
    vec.resize(bs.size() - offset);
    for(size_t i = 0; i < vec.size(); i++){
        vec[i] = bs[i + offset] ? 1 : 0;
    }
    return vec;
}
