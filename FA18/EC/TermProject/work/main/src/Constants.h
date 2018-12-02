// Configuration file for simple EA vs EcoEA testing
// 9/7/18
// Austin Ferguson

#pragma once

#include "./config/ConfigLoader.h"
#include "./config/BasicLoaders.h"

ConfigLoader* instance = nullptr;

void ConfigInit(ConfigLoader& config){
    instance = &config;
    config.Register("POP_SIZE", loader_int, delete_int);
    config.Register("NUM_GENS", loader_int, delete_int);
    config.Register("INIT_STEPS", loader_int, delete_int);
    config.Register("DATA_TRIM_FACTOR", loader_int, delete_int);

    config.Register("PCT_CARRY", loader_float, delete_float);
    config.Register("PCT_MUT", loader_float, delete_float);
    config.Register("PCT_CROSS", loader_float, delete_float);
    config.Register("MUT_RATE_SCALE", loader_float, delete_float);

    config.Register("WIDTH", loader_int, delete_int);
    config.Register("HEIGHT", loader_int, delete_int);
    config.Register("NUM_STEPS", loader_int, delete_int);
    config.Register("SUB_WIDTH", loader_int, delete_int);
    config.Register("SUB_HEIGHT", loader_int, delete_int);
    config.Register("SUB_X", loader_int, delete_int);
    config.Register("SUB_Y", loader_int, delete_int);
    config.Register("AUX_X", loader_int, delete_int);
    config.Register("AUX_Y", loader_int, delete_int);
    
    config.Register("AUTOMATON", loader_string, delete_string);
    config.Register("END_STATE", loader_string, delete_string);
    
    config.Register("TOURNEY_COUNT", loader_int, delete_int);
    config.Register("TOURNEY_SIZE", loader_int, delete_int);
    config.Register("ELITE_COUNT", loader_int, delete_int);
    config.Register("ELITE_COPIES", loader_int, delete_int);
    config.Register("NUM_RULESETS", loader_int, delete_int);
    config.Register("NUM_COLLABORATORS", loader_int, delete_int);
    
    config.Register("FIT_FUN", loader_string, delete_int);
    config.Register("CHECK_STEPS", loader_int, delete_int);
    config.Register("CAN_MOVE", loader_bool, delete_bool);
    config.Register("MUST_MOVE", loader_bool, delete_bool);
    config.Register("B_BLACK_MASK", loader_int, delete_int);
    config.Register("S_BLACK_MASK", loader_int, delete_int);

    config.Register("VIZ_ONLY", loader_bool, delete_bool);
    
    config.Register("OUTPUT_DIR", loader_string, delete_string);
}

ConfigLoader & GetConfig(){
    if(instance == nullptr)
        std::cerr << "Config not setup!" << std::endl;
    return *instance;
}
