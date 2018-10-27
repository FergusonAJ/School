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
            isToroidal = GetConfig().Fetch<int>("IS_TOROIDAL") == 1;
        }
        size_t Run(bool print = false, std::ostream& oss = std::cout){
            GPNode root = tree.GetRoot();
            if(print){
                Print(oss);
                oss << std::endl;
            }
            for(int i = 0; i < GetConfig().Fetch<int>("NUM_STEPS"); i++){
                EvaluateNode(root);
                if(print) {
                    Print(oss);
                    oss << std::endl;
                }
            }
            return foodCounter;
        }
        void Print(std::ostream& oss =std::cout){
            for(int j = 0; j < height; j++){
                for(int i = 0; i < width; i++){
                    if(i == x && j == y){
                        if(dir == NORTH)
                            oss << "^";
                        else if(dir == SOUTH)
                            oss << "v";
                        else if(dir == EAST)
                            oss << ">";
                        else if(dir == WEST)
                            oss << "<";
                    }
                    else{
                        oss << (int)trail.GetAt(i,j);
                    }
                }
                oss << std::endl;
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
                    MoveForward();
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
        void MoveForward(){
            if(isToroidal){
                if(dir == NORTH)
                    MoveTo(x, (y - 1) % height);
                else if(dir == SOUTH)
                    MoveTo(x, (y + 1) % height);
                else if(dir == EAST)
                    MoveTo((x + 1) % width, y);
                else if(dir == WEST)
                    MoveTo((x - 1) % width, y);
            }
            else{
                if(dir == NORTH){
                    MoveTo(x, (y == 0) ? y : y - 1);
                }
                else if(dir == SOUTH)
                    MoveTo(x, (y == height-1) ? y : y + 1);
                else if(dir == EAST)
                    MoveTo((x == height - 1) ? x : x + 1, y);
                else if(dir == WEST)
                    MoveTo((x == 0) ? x : x - 1, y);
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
        bool isToroidal = false;
        Trail trail;
};
