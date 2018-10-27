// Configuration file setup
// 9/7/18
// Austin Ferguson

#pragma once

#include "./ConfigLoader.h"
#include "./BasicLoaders.h"

ConfigLoader* instance = nullptr;

void ConfigInit(ConfigLoader& config){
    instance = &config;
    config.Register("POP_SIZE", loader_int, delete_int);
    config.Register("NUM_GENS", loader_int, delete_int);
    config.Register("TARGET_FITNESS", loader_float, delete_float);
    config.Register("DATA_TRIM_FACTOR", loader_int, delete_int);
    
    config.Register("OFFSPRING_FACTOR", loader_float, delete_float);
    config.Register("RANDOM_FACTOR", loader_float, delete_float);
    config.Register("KEEP_PARENTS", loader_int, delete_int);

    config.Register("CROSS_RATE", loader_float, delete_float);
    config.Register("MUT_RATE", loader_float, delete_float);

    config.Register("TREE_MAX_DEPTH", loader_int, delete_int);
    config.Register("TREE_MAX_INIT_DEPTH", loader_int, delete_int);

    config.Register("TRAIL_FILENAME", loader_string, delete_string);
    config.Register("WIDTH", loader_int, delete_int);
    config.Register("HEIGHT", loader_int, delete_int);
    config.Register("NUM_STEPS", loader_int, delete_int);
    config.Register("IS_TOROIDAL", loader_int, delete_int);
}

ConfigLoader & GetConfig(){
    if(instance == nullptr)
        std::cerr << "Config not setup!" << std::endl;
    return *instance;
}

