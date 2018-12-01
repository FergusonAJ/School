#pragma once
#include "OrganismShared.h"
#include "Evolve/World.h"
#include "./Classic.h"

//Variables
emp::World<emp::vector<bool>>* icWorldPtrs[4];

//Utility functions
void SetICQuadPtr(emp::World<emp::vector<bool>>* ptr, size_t idx){
    icWorldPtrs[idx] = ptr;
}

//Get random organism functions
std::vector<bool> GetRandomOrg_Quad_IC(emp::Random& rand){
    std::vector<bool> vec(width * height / 4);
    for(size_t i = 0; i < vec.size(); i++){
        vec[i] = (spawnFunc(rand) == 1);
    }
    return vec;
}

//Print functions
std::function<void(std::vector<bool> &, std::ostream &)> print_fun_quad_ic = [](std::vector<bool> & org, std::ostream & os) {
    for(int y = 0; y < subHeight / 2; y++){
        for(int x = 0; x < subWidth / 2; x++){
            os << (org[x + y * subWidth]? "1" : "0") << " ";
        }
        os << std::endl;
    }
    os << std::endl;
};

//Fitness functions
auto fit_fun_match_quad_ic = [](std::vector<bool> & org){
    ca.Reset();
    std::vector<unsigned char> tmp = BitstringToVec(org, 18);
    ca.AddSubsurface(tmp, subX, subY, subWidth);
    return GetMatchFitness();
};

auto fit_fun_static_rep_quad_ic_1 = [](std::vector<bool> & org){
    ca.Reset();
    std::vector<unsigned char> tmp = BitstringToVec(org);
    double max = GetStaticRepFitness();
    double score = 0;
    size_t idx = 0;
    for(int i = 0; i < numCollaborators; i++){
        ca.Reset();
        ca.AddSubsurface(tmp, 0, 0, subWidth/2);
        idx = randPtr->GetUInt(0, rulesetWorldPtr->GetSize());    
        ca.SetUpdateFunc(GetUpdateFunc(rulesetWorldPtr->GetOrg(idx)));
        score = GetStaticRepFitness();
        if(score > max)
            max = score;
    }
    return max;
};
auto fit_fun_static_rep_quad_ic_2 = [](std::vector<bool> & org){
    ca.Reset();
    std::vector<unsigned char> tmp = BitstringToVec(org);
    double max = GetStaticRepFitness();
    double score = 0;
    size_t idx = 0;
    for(int i = 0; i < numCollaborators; i++){
        ca.Reset();
        ca.AddSubsurface(tmp, subWidth / 2, 0, subWidth/2);
        idx = randPtr->GetUInt(0, rulesetWorldPtr->GetSize());    
        ca.SetUpdateFunc(GetUpdateFunc(rulesetWorldPtr->GetOrg(idx)));
        score = GetStaticRepFitness();
        if(score > max)
            max = score;
    }
    return max;
};
auto fit_fun_static_rep_quad_ic_3 = [](std::vector<bool> & org){
    ca.Reset();
    std::vector<unsigned char> tmp = BitstringToVec(org);
    double max = GetStaticRepFitness();
    double score = 0;
    size_t idx = 0;
    for(int i = 0; i < numCollaborators; i++){
        ca.Reset();
        ca.AddSubsurface(tmp, 0, subHeight / 2, subWidth/2);
        idx = randPtr->GetUInt(0, rulesetWorldPtr->GetSize());    
        ca.SetUpdateFunc(GetUpdateFunc(rulesetWorldPtr->GetOrg(idx)));
        score = GetStaticRepFitness();
        if(score > max)
            max = score;
    }
    return max;
};
auto fit_fun_static_rep_quad_ic_4 = [](std::vector<bool> & org){
    ca.Reset();
    std::vector<unsigned char> tmp = BitstringToVec(org);
    double max = GetStaticRepFitness();
    double score = 0;
    size_t idx = 0;
    for(int i = 0; i < numCollaborators; i++){
        ca.Reset();
        ca.AddSubsurface(tmp, subWidth / 2, subHeight / 2, subWidth/2);
        idx = randPtr->GetUInt(0, rulesetWorldPtr->GetSize());    
        ca.SetUpdateFunc(GetUpdateFunc(rulesetWorldPtr->GetOrg(idx)));
        score = GetStaticRepFitness();
        if(score > max)
            max = score;
    }
    return max;
};

auto fit_fun_static_rep_quad_ruleset = [](std::vector<bool> & org){
    ca.Reset();
    ca.SetUpdateFunc(GetUpdateFunc(org));
    //Check against most fit individual
    double max = 0;
    double score = 0;
    size_t idx = 0;
    std::vector<unsigned char> tmp;
    for(int i = 0; i < numCollaborators; i++){  
        ca.Reset();
        //1 
        idx = randPtr->GetUInt(0, icWorldPtrs[0]->GetSize());   
        tmp = BitstringToVec(icWorldPtrs[0]->GetOrg(idx));
        ca.AddSubsurface(tmp, 0, 0, subWidth / 2);
        //2 
        idx = randPtr->GetUInt(0, icWorldPtrs[1]->GetSize());   
        tmp = BitstringToVec(icWorldPtrs[1]->GetOrg(idx));
        ca.AddSubsurface(tmp, subWidth / 2, 0, subWidth / 2);
        //3 
        idx = randPtr->GetUInt(0, icWorldPtrs[2]->GetSize());   
        BitstringToVec(icWorldPtrs[2]->GetOrg(idx));
        ca.AddSubsurface(tmp, 0, subHeight / 2, subWidth / 2);
        //4 
        idx = randPtr->GetUInt(0, icWorldPtrs[3]->GetSize());   
        BitstringToVec(icWorldPtrs[3]->GetOrg(idx));
        ca.AddSubsurface(tmp, subWidth / 2, subHeight / 2,  subWidth / 2);
        score = GetStaticRepFitness();
        if(score > max)
            max = score;
    }
    return max;
};
