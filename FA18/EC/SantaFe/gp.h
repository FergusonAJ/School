#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>

#include "./Config.h"
#include "./trail.h"
#include "./gptree.h"
#include "./gprun.h"


struct GPInd{
    GPTree tree; 
    double fitness; 
    double prob;
    GPInd(GPTree tree_, double fit_, double prob_ = 0):
        tree(tree_),
        fitness(fit_),
        prob(prob_){
    };
};


inline bool compareHelper(GPInd a, GPInd b) {
    return (a.fitness < b.fitness);
}

class GPManager{
public:
    GPManager(){
        width = GetConfig().Fetch<int>("WIDTH"); 
        height = GetConfig().Fetch<int>("HEIGHT"); 
        popSize = GetConfig().Fetch<int>("POP_SIZE"); 
        numGens = GetConfig().Fetch<int>("NUM_GENS"); 
        maxDepth = GetConfig().Fetch<int>("TREE_MAX_DEPTH"); 
        maxInitDepth = GetConfig().Fetch<int>("TREE_MAX_INIT_DEPTH"); 
        crossRate = GetConfig().Fetch<float>("CROSS_RATE");
        mutRate = GetConfig().Fetch<float>("MUT_RATE");
        offspringFactor = GetConfig().Fetch<float>("OFFSPRING_FACTOR");
        randomFactor = GetConfig().Fetch<float>("RANDOM_FACTOR");
        keepParents = (GetConfig().Fetch<int>("KEEP_PARENTS") == 1);
        trail.LoadFromFile(GetConfig().Fetch<std::string>("TRAIL_FILENAME"), width, height);
    }
    
    void Run(){
        std::ofstream fitFP, orgFP, bestFP, selectFP, rankFP;
        fitFP.open("fitness.csv", std::ios::out | std::ios::trunc);
        fitFP << "gen,best,avg,worst\n";
        orgFP.open("orgList.txt", std::ios::out| std::ios::trunc);
        bestFP.open("best.txt", std::ios::out| std::ios::trunc);
        selectFP.open("select.txt", std::ios::out | std::ios::trunc);
        InitPop();
        std::cout << "Start:" << std::endl;
        SortPop();
        RankPop();
        for(int i = pop.size() - 1; i < pop.size(); i++){
            std::cout << pop[i].fitness << std::endl;
            std::cout << pop[i].tree.GetString() << std::endl;
            //GPRun gpRun(pop[i].tree, trail);
            //gpRun.Run(true);
        }
        for(int gen = 0; gen < numGens; gen++){
            std::cout << gen << " - " << trail.GetTotalFood() << std::endl;
            std::vector<int> idxVec = ParentSelect(popSize);
            int idxA, idxB;
            offspring.clear();
            while(offspring.size() < popSize * offspringFactor){
                idxA = floor(((float)rand() / RAND_MAX) * idxVec.size());
                GPInd A = pop[idxVec[idxA]];
                idxB = floor(((float)rand() / RAND_MAX) * idxVec.size());
                GPInd B = pop[idxVec[idxB]];
//                std::cout << idxVec[idxA] << " x " << idxVec[idxB] << std::endl;
//                selectFP << idxVec[idxA] << "\n" << idxVec[idxB] << "\n";
                if((float)rand() / RAND_MAX > crossRate){
                    offspring.push_back(A);
                    offspring.push_back(B);
                }
                else{
                    std::tuple<GPTree, GPTree> res = A.tree.Cross(B.tree, maxDepth);
                    GPInd A_ = GPInd(std::get<0>(res), 0, 0);    
                    GPRun gpRun_A(A_.tree, trail);
                    A_.fitness = (double)gpRun_A.Run() / trail.GetTotalFood();
                    GPInd B_ = GPInd(std::get<0>(res), 0, 0);    
                    GPRun gpRun_B(B_.tree, trail);
                    B_.fitness = (double)gpRun_B.Run() / trail.GetTotalFood();
                    offspring.push_back(A_);
                    offspring.push_back(B_);
                }
            }
            while(offspring.size() < popSize * (offspringFactor + randomFactor)){
                GPTree tree(maxDepth, maxInitDepth);
                GPRun gpRun(tree, trail);
                double fitness = (double)gpRun.Run() / trail.GetTotalFood();
                offspring.push_back(GPInd(tree, fitness, 0)); 
            }
            if(!keepParents)
                pop.clear();
            for(GPInd ind : offspring){
                if((float)rand() / RAND_MAX < mutRate){
                    GPRun gpRun(ind.tree, trail);
                    ind.tree.Mutate(maxDepth);
                    ind.fitness = (double)gpRun.Run() / trail.GetTotalFood();
                }
                pop.push_back(ind);
            }
            SortPop();
            RankPop();
            pop.erase(pop.begin(), pop.end() - popSize);
            float sum = 0;
            for(GPInd ind : pop){
                sum += ind.fitness;
            }
            fitFP << gen << ",";
            fitFP << pop[pop.size() - 1].fitness << ",";
            fitFP << (sum / pop.size()) << ",";
            fitFP << pop[0].fitness  << "\n";
            /*
            orgFP << "GEN: " << gen << "\n";
            for(int i = 0; i < pop.size(); i++){
                orgFP << pop[i].fitness << "\n";
                orgFP << pop[i].tree.GetString() << "\n\n";
            }
            orgFP << "\n\n\n";
            */
        }
        std::cout << "End:" << std::endl;
        for(int i = pop.size() - 1; i < pop.size(); i++){
            std::cout << pop[i].fitness << std::endl;
            std::cout << pop[i].tree.GetString() << std::endl;
            bestFP << pop[i].fitness << std::endl;
            bestFP << pop[i].tree.GetString() << std::endl;
            GPRun gpRun(pop[i].tree, trail);
            int food = gpRun.Run(true, bestFP);
            bestFP << "Food eaten: " << food << " / " << trail.GetTotalFood();
            
        }
        fitFP.close();
        orgFP.close();
        bestFP.close();
        selectFP.close();
    }

private:
    void InitPop(){
        for(int i = 0; i < popSize; i++){
            GPTree tree(maxDepth, maxInitDepth);
            GPRun gpRun(tree, trail);
            double fitness = (double)gpRun.Run() / trail.GetTotalFood();
            pop.push_back(GPInd(tree, fitness, 0)); 
        }
    }
    
    void SortPop(){
        std::sort(pop.begin(), pop.end(), compareHelper);
    }

    void RankPop(){
        for(int i = 0; i < pop.size(); i++){
            pop[i].prob = (1 - exp(-i)) / pop.size();
        }
    }
    
    std::vector<int> ParentSelect(int k){
        std::vector<int> vec;
        vec.resize(k, 0);
        double rnd = (double)rand() / RAND_MAX * (1/k);
        double sum = 0;
        double threshold = 0;
        for(int j = 0; j < k; j++){
            sum = 0;
            threshold = rnd + j * (1.0/k);
            if (threshold > 1)
                threshold -= 1.0;
            for (size_t i = 0; i < pop.size(); i++){
                sum += pop[i].prob;
                if(sum > threshold){
                    vec[j] = i;
                    break;
                }
            }
        }
        return vec;
    }
    
    int SelectIndex(){
        double rnd = (double)rand() / RAND_MAX;
        double sum = 0;
        for (size_t i = 0; i < pop.size(); i++){
            sum += pop[i].prob;
            if(sum > rnd){
                return i;
            }
        }
        return 0;
    }

private:
    size_t width = 0;
    size_t height = 0;
    size_t popSize = 0;
    size_t numGens = 0;
    size_t maxDepth = 0;
    size_t maxInitDepth = 0;
    float crossRate = 0;
    float mutRate = 0;
    float offspringFactor = 0;
    float randomFactor = 0;
    bool keepParents = false;
    std::vector<GPInd> pop;
    std::vector<GPInd> offspring;
    Trail trail;
};
