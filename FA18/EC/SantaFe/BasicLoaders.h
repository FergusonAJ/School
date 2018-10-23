// Basic loaders to be used with ConfigLoader.h
// Note: All possible config values must be registered prior!
// Austin Ferguson
// 10/2/2018

#pragma once
#include <string>

//Integers
std::function<void *(std::string&)> loader_int =
    [](std::string& s){
        int* tmp = new int;
        *tmp = std::stoi(s);
        return (void*)tmp;
};
std::function<void(void*)> delete_int = 
    [](void* p){
       delete (int*) p;
};

//Floats
std::function<void *(std::string&)> loader_float =
    [](std::string& s){
        float* tmp = new float;
        *tmp = std::stof(s);
        return (void*)tmp;
};
std::function<void(void*)> delete_float = 
    [](void* p){
       delete (float*) p;
};

//Chars
std::function<void *(std::string&)> loader_char =
    [](std::string& s){
        char* tmp = new char;
        *tmp = s[0];
        return (void*)tmp;
};
std::function<void(void*)> delete_char = 
    [](void* p){
       delete (char*) p;
};

//Strings
std::function<void *(std::string&)> loader_string =
    [](std::string& s){
        std::string* newS = new std::string(s);
        return (void*)newS;
};
std::function<void(void*)> delete_string = 
    [](void* p){
       delete (std::string*) p;
};
