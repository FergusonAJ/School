#pragma once
    
#include "./gpnode.h"

class GPTree{
public:
    GPTree(int maxDepth_, int maxInitialDepth_):
        maxDepth(maxDepth_),
        maxInitialDepth(maxInitialDepth_),
        root(GPNode(maxInitialDepth, 1)){
    }
    std::string GetString(){
        std::ostringstream oss;
        root.GetString(oss);
        return oss.str();
    }
    GPNode GetRoot() const{
        return root;
    }
    void Mutate(int maxDepth){
        root.Mutate(maxDepth);
    }
    std::tuple<GPTree, GPTree> Cross(GPTree& other, int maxDepth){
        std::tuple<GPNode*, int> A = GetCrossoverPoint(); 
        std::tuple<GPNode*, int> B = other.GetCrossoverPoint();
        if(std::get<1>(A) == -1 || std::get<1>(B) == -1){
            return std::make_tuple(*this, other);
        }
        else{
           GPNode A_c = std::get<0>(A)->GetChild(std::get<1>(A));
           GPNode B_c = std::get<0>(B)->GetChild(std::get<1>(B));
           std::get<0>(A)->SetChild(std::get<1>(A), B_c);
           std::get<0>(B)->SetChild(std::get<1>(B), A_c);
           Prune(maxDepth);
           other.Prune(maxDepth);
        }
    }
    std::tuple<GPNode*, int> GetCrossoverPoint(){
        return root.GetCrossoverPoint();
    }
    void Prune(int maxDepth){
        root.Prune(maxDepth);
    }
private:
    GPNode root;
    int maxDepth, maxInitialDepth; 
};

