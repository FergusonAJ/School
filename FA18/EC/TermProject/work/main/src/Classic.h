#pragma once
#include "OrganismShared.h"
#include "Evolve/World.h"

//Variables
emp::World<emp::vector<bool>>* icWorldPtr;
emp::World<emp::vector<bool>>* rulesetWorldPtr;
size_t maxFitIC = 0;

//Utility functions
void SetICWorldPtr(emp::World<emp::vector<bool>>* ptr){
    icWorldPtr = ptr;
}
void SetRulesetWorldPtr(emp::World<emp::vector<bool>>* ptr){
    rulesetWorldPtr = ptr;
}
void SetMaxFitIC(size_t idx){
    maxFitIC = idx;
}

//Get random organism functions
std::vector<bool> GetRandomOrg_Classic_IC(emp::Random& rand){
    std::vector<bool> vec(width * height);
    for(size_t i = 0; i < vec.size(); i++){
        vec[i] = (spawnFunc(rand) == 1);
    }
    return vec;
}

std::vector<bool> GetRandomOrg_Classic_Ruleset(emp::Random& rand){
    std::vector<bool> vec(18);
    for(size_t i = 0; i < vec.size(); i++){
        vec[i] = (spawnFunc(rand) == 1);
    }
    return vec;
}

//Print functions
std::function<void(std::vector<bool> &, std::ostream &)> print_fun_classic_ic = [](std::vector<bool> & org, std::ostream & os) {
    for(int y = 0; y < subHeight; y++){
        for(int x = 0; x < subWidth; x++){
            os << (org[x + y * subWidth]? "1" : "0") << " ";
        }
        os << std::endl;
    }
    os << std::endl;
};

std::function<void(std::vector<bool> &, std::ostream &)> print_fun_classic_ruleset = [](std::vector<bool> & org, std::ostream & os) {
    os << "B";
    for(int i = 0; i < 9; i++){
        if(org[i])
            os << i;
    }
    os << "/S";
    for(int i = 0; i < 9; i++){
        if(org[i+9])
            os << i;
    }
    os << std::endl;
};


//Fitness functions
auto fit_fun_match_classic_ic = [](std::vector<bool> & org){
    ca.Reset();
    std::vector<unsigned char> tmp = BitstringToVec(org, 18);
    ca.AddSubsurface(tmp, subX, subY, subWidth);
    return GetMatchFitness();
};

auto fit_fun_static_rep_classic_ic = [](std::vector<bool> & org){
    ca.Reset();
    std::vector<unsigned char> tmp = BitstringToVec(org);
    double max = GetStaticRepFitness();
    double score = 0;
    size_t idx = 0;
    for(int i = 0; i < numCollaborators; i++){
        ca.Reset();
        ca.AddSubsurface(tmp, subX, subY, subWidth);
        idx = randPtr->GetUInt(0, rulesetWorldPtr->GetSize());    
        ca.SetUpdateFunc(GetUpdateFunc(rulesetWorldPtr->GetOrg(idx)));
        score = GetStaticRepFitness();
        if(score > max)
            max = score;
    }
    return max;
};

auto fit_fun_match_classic_ruleset = [](std::vector<bool> & org){
    ca.Reset();
    ca.SetUpdateFunc(GetUpdateFunc(org));
    std::vector<unsigned char> tmp = BitstringToVec(org, 18);
    ca.AddSubsurface(tmp, subX, subY, subWidth);
    return GetMatchFitness();
};

auto fit_fun_static_rep_classic_ruleset = [](std::vector<bool> & org){
    ca.Reset();
    ca.SetUpdateFunc(GetUpdateFunc(org));
    //Check against most fit individual
    std::vector<unsigned char> tmp = BitstringToVec(icWorldPtr->GetOrg(maxFitIC));
    ca.AddSubsurface(tmp, subX, subY, subWidth);
    double max = GetStaticRepFitness();
    double score = 0;
    size_t idx = 0;
    for(int i = 0; i < numCollaborators - 1; i++){  
        ca.Reset(); 
        idx = randPtr->GetUInt(0, icWorldPtr->GetSize());   
        std::vector<unsigned char> tmp = BitstringToVec(icWorldPtr->GetOrg(idx));
        ca.AddSubsurface(tmp, subX, subY, subWidth);
        score = GetStaticRepFitness();
        if(score > max)
            max = score;
    }
    return max;
};
