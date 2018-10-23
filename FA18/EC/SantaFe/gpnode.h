#pragma once

#include <vector>
#include <math.h>
#include <string>
#include <iostream>
#include <sstream>
#include <tuple>

#define NODE_IF 1
#define NODE_RIGHT 2
#define NODE_LEFT 3
#define NODE_FORWARD 4
#define NODE_IS_FACING_FOOD 5
#define NODE_SEQ 6

#define NUM_NODE_TYPES 6

struct GPNodeFunc{
    GPNodeFunc(int nType, int numC){
        nodeType = nType;
        numChildren = numC;
    };
    GPNodeFunc(){
    };
    int nodeType;
    int numChildren;
};

GPNodeFunc funcArr[] = 
{
    {NODE_IF, 3},
    {NODE_RIGHT, 0},
    {NODE_LEFT, 0},
    {NODE_FORWARD, 0},
    {NODE_IS_FACING_FOOD, 0},
    {NODE_SEQ, 2}
};

class GPNode{
public:
    GPNode(int maxDepth, int depth_):
        depth(depth_){
            Grow(maxDepth);
    }
    ~GPNode(){
    }
    void GetString(std::ostringstream& oss) const{
        for(int i = 0; i < depth; i++)
            oss <<  "  ";
        oss <<  (int)func.nodeType << " (" << depth << ")" <<  "\n";
        for(int i = 0; i < children.size(); i++){
            children[i].GetString(oss);
        } 
    }
    size_t GetChildCount() const{
        return children.size();
    }
    GPNode GetChild(int idx) const{
        return children[idx];
    }
    void SetChild(int idx, GPNode child){
        if(idx < children.size())
            children[idx] = child;
    }
    size_t GetType() const{
        return func.nodeType;
    }
    void Grow(int maxDepth){
        int idx = floor(((double)rand() / (RAND_MAX)) * NUM_NODE_TYPES);
        func = funcArr[idx];
        while(func.numChildren > 0 && depth == maxDepth){
            idx = floor(((double)rand() / (RAND_MAX)) * NUM_NODE_TYPES);
            func = funcArr[idx];
        }
        if(func.numChildren > 0){
            while(children.size() < func.numChildren){
                children.push_back(GPNode(maxDepth, depth + 1));
            }
        }
    }
    void Mutate(int maxDepth){
        int numDescendents = GetNumDescendents();
        if(numDescendents == 0){
           Grow(maxDepth); 
        }
        else{
            float thresh = 1.0f / (numDescendents);
            MutateHelper(maxDepth, thresh);
        }
    }
    bool MutateHelper(int maxDepth, float threshold){
        for(int i = 0; i < children.size(); i++){
            if((float)rand() / RAND_MAX < threshold){
                children[i] = GPNode(maxDepth, depth + 1);
                return true;
            }
            if(MutateHelper(maxDepth, threshold))
                return true;
        }
        return false;
    }
    std::tuple<GPNode*, int> GetCrossoverPoint(){
        int numDescendents = GetNumDescendents();
        if(numDescendents == 0){
           return std::make_tuple(this, -1); 
        }
        float thresh = 1.0f / (numDescendents);
        std::tuple<GPNode*, int> helper = CrossPointHelper(thresh);
        if(std::get<1>(helper) != -1){
           return std::make_tuple(this, -1); 
        }
    }
    std::tuple<GPNode*, int> CrossPointHelper(float threshold){
        for(int i = 0; i < children.size(); i++){
            if((float)rand() / RAND_MAX < threshold){
                return std::make_tuple(this, i);;
            }
            std::tuple<GPNode*, int> helper = children[i].CrossPointHelper(threshold);
            if(std::get<1>(helper) != -1){
                return helper;
            }
        }
        return std::make_tuple(this, -1); 
        
    }
    int GetNumDescendents(){
        int sum = children.size();
        for(int i = 0; i < children.size(); i++){
            sum += children[i].GetNumDescendents();
        }
        return sum;
    }
    void Prune(int maxDepth){
        for(int i = 0; i < children.size(); i++){
            children[i].Prune(maxDepth);
        }   
        if(depth >= maxDepth){
            children.clear();
        }
        else if(depth == maxDepth - 1){
            for(int i = 0; i < children.size(); i++){
                int idx = floor(((double)rand() / (RAND_MAX)) * NUM_NODE_TYPES);
                GPNodeFunc f = funcArr[idx];
                while(f.numChildren > 0){
                    idx = floor(((double)rand() / (RAND_MAX)) * NUM_NODE_TYPES);
                    f = funcArr[idx];
                }
                children[idx].SetFunc(funcArr[idx], maxDepth);
            }   
        }
    }
    void SetFunc(GPNodeFunc& f, int maxDepth){
        func = f;
        if(func.numChildren > 0){
            while(children.size() < func.numChildren){
                children.push_back(GPNode(maxDepth, depth + 1));
            }
        }
    }
private:
    std::vector<GPNode> children;
    GPNodeFunc func;
    int depth;
    
};
