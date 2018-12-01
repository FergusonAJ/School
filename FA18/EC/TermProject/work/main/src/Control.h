#include "OrganismShared.h"

std::vector<bool> GetRandomOrg_Control(emp::Random& rand){
    std::vector<bool> vec(width * height + 18);
    for(size_t i = 0; i < vec.size(); i++){
        vec[i] = (spawnFunc(rand) == 1);
    }
    return vec;
}

std::function<void(std::vector<bool> &, std::ostream &)> print_fun_control = [](std::vector<bool> & org, std::ostream & os) {
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
    os << "\n";
    for(int y = 0; y < subHeight; y++){
        for(int x = 0; x < subWidth; x++){
            os << (org[x + y * subWidth + 18]? "1" : "0") << " ";
        }
        os << std::endl;
    }
    os << std::endl;
};

auto fit_fun_match_control = [](std::vector<bool> & org){
    ca.Reset();
    ca.SetUpdateFunc(GetUpdateFunc(org));
    std::vector<unsigned char> tmp = BitstringToVec(org, 18);
    ca.AddSubsurface(tmp, subX, subY, subWidth);
    return GetMatchFitness();
};

auto fit_fun_static_rep_control = [](std::vector<bool> & org){
    ca.Reset();
    ca.SetUpdateFunc(GetUpdateFunc(org));
    std::vector<unsigned char> tmp = BitstringToVec(org, 18);
    ca.AddSubsurface(tmp, subX, subY, subWidth);
    double score = GetStaticRepFitness(); 
    return score;
};
