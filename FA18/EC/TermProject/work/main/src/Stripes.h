#pragma once
#include "OrganismShared.h"
#include "Evolve/World.h"
#include "./Quads.h"


//Print functions
std::function<void(std::vector<bool> &, std::ostream &)> print_fun_stripe_ic = [](std::vector<bool> & org, std::ostream & os) {
    for(int y = 0; y < subHeight / 4; y++){
        for(int x = 0; x < subWidth; x++){
            os << (org[x + y * subWidth]? "1" : "0") << " ";
        }
        os << std::endl;
    }
    os << std::endl;
};

//Fitness functions
auto fit_fun_match_stripe_ic = [](std::vector<bool> & org){
    ca.Reset();
    std::vector<unsigned char> tmp = BitstringToVec(org, 18);
    ca.AddSubsurface(tmp, subX, subY, subWidth);
    return GetMatchFitness();
};

auto fit_fun_static_rep_stripe_ic_1 = [](std::vector<bool> & org){
    ca.Reset();
    std::vector<unsigned char> tmp = BitstringToVec(org);
    double max = GetStaticRepFitness();
    double score = 0;
    size_t idx = 0;
    for(int i = 0; i < numCollaborators; i++){
        ca.Reset();
        ca.AddSubsurface(tmp, 0, 0, subWidth);
        idx = randPtr->GetUInt(0, rulesetWorldPtr->GetSize());    
        ca.SetUpdateFunc(GetUpdateFunc(rulesetWorldPtr->GetOrg(idx)));
        score = GetStaticRepFitness();
        if(score > max)
            max = score;
    }
    return max;
};
auto fit_fun_static_rep_stripe_ic_2 = [](std::vector<bool> & org){
    ca.Reset();
    std::vector<unsigned char> tmp = BitstringToVec(org);
    double max = GetStaticRepFitness();
    double score = 0;
    size_t idx = 0;
    for(int i = 0; i < numCollaborators; i++){
        ca.Reset();
        ca.AddSubsurface(tmp, 0, subHeight / 4, subWidth);
        idx = randPtr->GetUInt(0, rulesetWorldPtr->GetSize());    
        ca.SetUpdateFunc(GetUpdateFunc(rulesetWorldPtr->GetOrg(idx)));
        score = GetStaticRepFitness();
        if(score > max)
            max = score;
    }
    return max;
};
auto fit_fun_static_rep_stripe_ic_3 = [](std::vector<bool> & org){
    ca.Reset();
    std::vector<unsigned char> tmp = BitstringToVec(org);
    double max = GetStaticRepFitness();
    double score = 0;
    size_t idx = 0;
    for(int i = 0; i < numCollaborators; i++){
        ca.Reset();
        ca.AddSubsurface(tmp, 0, subHeight / 2, subWidth);
        idx = randPtr->GetUInt(0, rulesetWorldPtr->GetSize());    
        ca.SetUpdateFunc(GetUpdateFunc(rulesetWorldPtr->GetOrg(idx)));
        score = GetStaticRepFitness();
        if(score > max)
            max = score;
    }
    return max;
};
auto fit_fun_static_rep_stripe_ic_4 = [](std::vector<bool> & org){
    ca.Reset();
    std::vector<unsigned char> tmp = BitstringToVec(org);
    double max = GetStaticRepFitness();
    double score = 0;
    size_t idx = 0;
    for(int i = 0; i < numCollaborators; i++){
        ca.Reset();
        ca.AddSubsurface(tmp, 0, 3 * subHeight / 4, subWidth);
        idx = randPtr->GetUInt(0, rulesetWorldPtr->GetSize());    
        ca.SetUpdateFunc(GetUpdateFunc(rulesetWorldPtr->GetOrg(idx)));
        score = GetStaticRepFitness();
        if(score > max)
            max = score;
    }
    return max;
};

auto fit_fun_static_rep_stripe_ruleset = [](std::vector<bool> & org){
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
        ca.AddSubsurface(tmp, 0, 0, subWidth);
        //2 
        idx = randPtr->GetUInt(0, icWorldPtrs[1]->GetSize());   
        tmp = BitstringToVec(icWorldPtrs[1]->GetOrg(idx));
        ca.AddSubsurface(tmp, 0, subHeight / 4, subWidth);
        //3 
        idx = randPtr->GetUInt(0, icWorldPtrs[2]->GetSize());   
        BitstringToVec(icWorldPtrs[2]->GetOrg(idx));
        ca.AddSubsurface(tmp, 0, subHeight / 2, subWidth);
        //4 
        idx = randPtr->GetUInt(0, icWorldPtrs[3]->GetSize());   
        BitstringToVec(icWorldPtrs[3]->GetOrg(idx));
        ca.AddSubsurface(tmp, 0, 3 * subHeight / 4,  subWidth);
        score = GetStaticRepFitness();
        if(score > max)
            max = score;
    }
    return max;
};
