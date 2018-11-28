#pragma once

#include <functional>

#include "tools/string_utils.h"
#include "base/vector.h"

#include "../../../CellularAutomata/CellularAutomaton.h"
#include "../../../CellularAutomata/AllCAs.h"

#include "./Constants.h"
#include "./endLoader.h"
#include "./DefinitionDecoder.h"
#include "./util.h"


empCA::CellularAutomaton<unsigned char> ca;
empCA::Visualizer<unsigned char> viz;
std::function<unsigned char(emp::Random &)> spawnFunc;
int width, height, subWidth, subHeight, subX, subY, numSteps, checkSteps, numCollaborators;
bool canMove, mustMove;
std::string name;
float mutRate;
std::vector<unsigned char> endState;
int max = 0;
emp::Random* randPtr;

void Initialize(emp::Random* ptr){
    width = GetConfig().Fetch<int>("WIDTH");
    height = GetConfig().Fetch<int>("HEIGHT");
    subWidth = GetConfig().Fetch<int>("SUB_WIDTH");
    subHeight = GetConfig().Fetch<int>("SUB_HEIGHT");
    subX = GetConfig().Fetch<int>("SUB_X");
    subY = GetConfig().Fetch<int>("SUB_Y");
    numSteps = GetConfig().Fetch<int>("NUM_STEPS");
    mutRate = (1.0f / GetConfig().Fetch<float>("MUT_RATE_RECIPROCAL"));
    name = GetConfig().Fetch<std::string>("AUTOMATON");
    checkSteps = GetConfig().Fetch<int>("CHECK_STEPS");
    numCollaborators = GetConfig().Fetch<int>("NUM_COLLABORATORS");
    canMove = GetConfig().Fetch<bool>("CAN_MOVE");
    mustMove = GetConfig().Fetch<bool>("MUST_MOVE");
    LoadFile(endState, GetConfig().Fetch<std::string>("END_STATE"), width, height);
    ca.Resize(width, height);
    ca.SetFunctions(conwayStruct);
    spawnFunc = conwayStruct.func_spawn;
    randPtr = ptr;
}

void VisualizerInit(empCA::Visualizer<unsigned char>& v){
    viz.SetFunctions(conwayStruct);
}

empCA::Visualizer<unsigned char>* GetVisualizer(size_t width, size_t height){
    viz.Init(width, height, &ca);
    VisualizerInit(viz);
    return &viz;
}

double GetMatchFitness(){
    for(int i = 0; i < numSteps; i++){
        ca.Update();
    } 
    emp::vector<emp::Ptr<unsigned char>> vec = ca.GetField();
    
    double score = 0;
    int w = width;
    int h = height;
    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            if(*vec[x + y * w] == endState[x + y * w])
                score++;
        }
    }
    return score / (width * height);
}

double GetStaticRepFitness(){
    for(int i = 0; i < numSteps; i++){
        ca.Update();
    } 
    ca.GroupStart();
    std::vector<size_t> validStructures = ca.CheckAllRepeating(checkSteps,canMove,mustMove);
    return (double)validStructures.size(); 
}
