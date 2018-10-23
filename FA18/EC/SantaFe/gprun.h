#pragma once

#include "./gptree.h"
#include "./Config.h"
#include "./trail.h"

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

class GPRun{
    public:
        GPRun(const GPTree& tree_, const Trail& trail_):
                tree(tree_),
                trail(Trail(trail_)){
            width = trail.GetWidth();
            height = trail.GetHeight();
            MoveTo(0,0);
        }
        size_t Run(bool print = false){
            GPNode root = tree.GetRoot();
            if(print) {
                Print();
                std::cout << std::endl;
            }
            for(int i = 0; i < GetConfig().Fetch<int>("NUM_STEPS"); i++){
                EvaluateNode(root);
                if(print) {
                    Print();
                    std::cout << std::endl;
                }
            }
            return foodCounter;
        }
        void Print(){
            for(int j = 0; j < height; j++){
                for(int i = 0; i < width; i++){
                    if(i == x && j == y){
                        if(dir == NORTH)
                            std::cout << "^";
                        else if(dir == SOUTH)
                            std::cout << "v";
                        else if(dir == EAST)
                            std::cout << ">";
                        else if(dir == WEST)
                            std::cout << "<";
                    }
                    else{
                        std::cout << (int)trail.GetAt(i,j);
                    }
                }
                std::cout << std::endl;
            }
        } 
    private:
        bool IsFacingFood(){
            switch(dir){
                case NORTH:
                    return (trail.GetAt(x, y-1) == 1);
                case SOUTH:
                    return (trail.GetAt(x, y+1) == 1);
                case EAST:
                    return (trail.GetAt(x+1, y) == 1);
                case WEST:
                    return (trail.GetAt(x-1, y) == 1);
                default:
                    return false;
            }
        }
        bool EvaluateNode(const GPNode& node){
            switch(node.GetType()){
                case NODE_RIGHT:
                    dir = (dir + 1) % 4;
                    return false;  
                case NODE_LEFT:
                    dir = (dir - 1) % 4;
                    return false;
                case NODE_FORWARD:
                    if(dir == NORTH)
                        MoveTo(x, (y - 1) % height);
                    else if(dir == SOUTH)
                        MoveTo(x, (y + 1) % height);
                    else if(dir == EAST)
                        MoveTo((x + 1) % width, y);
                    else if(dir == WEST)
                        MoveTo((x - 1) % width, y);
                    return false;
                case NODE_IS_FACING_FOOD:
                    return IsFacingFood();
                case NODE_IF:
                    if(EvaluateNode(node.GetChild(0))){
                        return EvaluateNode(node.GetChild(1));
                    }
                    return EvaluateNode(node.GetChild(2));
                case NODE_SEQ:
                    EvaluateNode(node.GetChild(0));
                    return EvaluateNode(node.GetChild(1));
                default:
                    return false;
                
            }
        }
        void MoveTo(size_t x_, size_t y_){
            x = x_;
            y = y_;
            if(trail.GetAt(x,y) == 1){
                foodCounter++;
                trail.SetAt(x,y,0);
            }
        }

    private:
        size_t x = 0;
        size_t y = 0; 
        size_t dir = SOUTH;
        size_t width = 0;
        size_t height = 0;
        size_t foodCounter = 0;
        const GPTree tree;
        Trail trail;
};
