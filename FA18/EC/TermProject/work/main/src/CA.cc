//  Evolutionary Computing Term Project
//  Investigating Cooperative Co-Evolution Modularity Using Cellular Automata
//  Austin J. Ferguson
//  11/9/2018


//  This file uses parts of Empirical, https://github.com/devosoft/Empirical
//  Copyright (C) Michigan State University, 2016-2017.
//  Released under the MIT Software license; see doc/LICENSE

#include <iostream>
#include <string>
#include <string.h>
#include <map>
#include <sstream>

#include "Evolve/World.h"
#include "tools/Random.h"
#include "tools/string_utils.h"
#include "../../../CellularAutomata/Visualizer.h"

#include "./Constants.h"
//#include "./Organism.h"
#include "./Control.h"
#include "./Classic.h"
#include "./config/ConfigLoader.h"
#include "./util.h"

int main(int argc, char ** argv)
{

    /*
    //Load Configuration
    ConfigLoader config;
    ConfigInit(config); 
    config.Load("config.txt");
     
    //Initialize organism variables
    emp::Random random;
    Initialize();
    
    bool vizOnly = GetConfig().Fetch<bool>("VIZ_ONLY");
    if(!vizOnly){ // Do evolution
        //Variable initialization
        std::vector<unsigned char> endState;
        size_t popSize = GetConfig().Fetch<int>("POP_SIZE"); 
        size_t numGens = GetConfig().Fetch<int>("NUM_GENS"); 
        size_t numSteps = GetConfig().Fetch<int>("NUM_STEPS"); 
        size_t tourneySize= GetConfig().Fetch<int>("TOURNEY_SIZE"); 
        size_t tourneyCount = GetConfig().Fetch<int>("TOURNEY_COUNT"); 
        size_t eliteCount = GetConfig().Fetch<int>("ELITE_COUNT"); 
        size_t eliteCopies = GetConfig().Fetch<int>("ELITE_COPIES");
        std::string fitFunStr = GetConfig().Fetch<std::string>("FIT_FUN");
     
        //World Setup
        emp::World<emp::vector<bool>> world(random);
        world.SetPopStruct_Mixed(false);
        world.SetPrintFun(print_fun_control);
        world.SetMutFun(mutate_fun_control);
        std::function<double(std::vector<bool> &)> fit_fun = fit_fun_match_control;
        if(fitFunStr.compare("STATIC_REP") == 0){
            fit_fun = fit_fun_static_rep_control;
            std::cout << "Using static and repeating structures for fitness." << std::endl;
        }
        else {   
            fit_fun = fit_fun_match_control;
            std::cout << "Using end state matching for fitness." << std::endl;
        }
        world.SetFitFun(fit_fun);
        world.SetupFitnessFile("./output/fitness.csv", true);
        //world.SetupPopulationFile("./output/pop.csv", true);
        world.SetCache(true);

        //Fill the world with random orgs
        for(size_t i = 0; i < popSize; i++){
            world.Inject(GetRandomOrg_Control(random));
        }
        world.Update();
        


        //Main Loop
        for(size_t i = 0; i < numGens; i++){
            std::cout << "Generation #" << i << std::endl;
            if(eliteCount > 0 && eliteCopies > 0)
                EliteSelect(world, eliteCount, eliteCopies);
            TournamentSelect(world, tourneySize, tourneyCount);
            world.DoMutations();
            world.Update();
        }

        // Last generation image generation
        std::cout << "Last generation finished! Finishing up..." << std::endl; 
        std::multimap<double, size_t> fit_map;
        for (size_t id = 0; id < world.GetSize(); id++) {
            double cur_fit = fit_fun(world.GetOrg(id));//world.CalcFitnessID(id);
            fit_map.insert( std::make_pair(cur_fit, id) );
        }
        auto viz = GetVisualizer(800,600);
        int sub_x = GetConfig().Fetch<int>("SUB_X");
        int sub_y = GetConfig().Fetch<int>("SUB_Y");
        int sub_w = GetConfig().Fetch<int>("SUB_WIDTH");
        auto m = fit_map.rbegin();
        std::cout << "Generating images..." << std::endl;
        std::ostringstream os;
        for(size_t i = 0; i < fit_map.size(); i++){
            ca.Reset(); 
            std::vector<unsigned char> tmp = BitstringToVec(world.GetOrg(m->second), 18);
            ca.AddSubsurface(tmp, sub_x, sub_y, sub_w);
            os.str("");
            os << "./output/images/" << i << "_" << m->first << "_start.png";
            viz->SaveImage(os.str());
            for(size_t j = 0; j < numSteps; j++)
                ca.Update();
            os.str("");
            os << "./output/images/" << i << "_" << m->first << "_stop.png";
            viz->SaveImage(os.str());
            m++;
        }

        //Visualizer
        ca.Reset();
        std::vector<unsigned char> tmp = BitstringToVec(world.GetOrg(fit_map.rbegin()->second), 18);
        ca.AddSubsurface(tmp, sub_x, sub_y, sub_w);
        viz->SaveImage("./output/Best.png");
        viz->Run();
        viz->LoadImage("./output/Best.png");
        viz->Run(); 
    }
    else{ //Viz only
        std::cout << "VIZ_ONLY flag set. Only running visualization of ./output/Best.png" <<std::endl;
        auto viz = GetVisualizer(800,600);
        viz->LoadImage("./output/Best.png");
        viz->Run(); 
    }
    */
    //Finished! 
    std::cout << "Done!" << std::endl;
}
