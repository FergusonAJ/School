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
#include "./Control.h"
#include "./Classic.h"
#include "./config/ConfigLoader.h"
#include "./util.h"

int main(int argc, char ** argv)
{
    //Load Configuration
    ConfigLoader config;
    ConfigInit(config); 
    config.Load("config.txt");
     
    //Initialize organism variables
    emp::Random random;
    Initialize(&random);
    
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
        size_t numRulesets = GetConfig().Fetch<int>("NUM_RULESETS");
        std::string fitFunStr = GetConfig().Fetch<std::string>("FIT_FUN");

     
        //IC World Setup
        emp::World<emp::vector<bool>> worldIC(random);
        worldIC.SetPopStruct_Mixed(true);
        worldIC.SetPrintFun(print_fun_classic_ic);
        worldIC.SetMutFun(mutate_fun_classic);
        std::function<double(std::vector<bool> &)> fit_fun_ic = fit_fun_match_classic_ic;
        
        if(fitFunStr.compare("STATIC_REP") == 0){
            fit_fun_ic = fit_fun_static_rep_classic_ic;
            std::cout << "Using static and repeating structures for fitness." << std::endl;
        }
        else {   
            fit_fun_ic = fit_fun_match_classic_ic;
            std::cout << "Using end state matching for fitness." << std::endl;
        }
        worldIC.SetFitFun(fit_fun_ic);
        worldIC.SetCache(true);

        //Fill the world with random orgs
        for(size_t i = 0; i < popSize; i++){
            worldIC.Inject(GetRandomOrg_Classic_IC(random));
        }
        worldIC.Update();
        SetICWorldPtr(&worldIC);
        
        //Ruleset World Setup
        emp::World<emp::vector<bool>> worldRuleset(random);
        worldRuleset.SetPopStruct_Mixed(true);
        worldRuleset.SetPrintFun(print_fun_classic_ruleset);
        worldRuleset.SetMutFun(mutate_fun_classic);
        std::function<double(std::vector<bool> &)> fit_fun_ruleset = fit_fun_match_classic_ruleset;
        
        if(fitFunStr.compare("STATIC_REP") == 0){
            fit_fun_ruleset = fit_fun_static_rep_classic_ruleset;
        }
        else {   
            fit_fun_ruleset = fit_fun_match_classic_ruleset;
        }
        worldRuleset.SetFitFun(fit_fun_ruleset);
        worldRuleset.SetCache(true);

        //Fill the world with random orgs
        for(size_t i = 0; i < numRulesets; i++){
            worldRuleset.Inject(GetRandomOrg_Classic_Ruleset(random));
        }
        worldRuleset.Update();
        SetRulesetWorldPtr(&worldRuleset);

        //worldIC.SetupFitnessFile("./output/fitness_ic.csv", true);
        //worldRuleset.SetupFitnessFile("./output/fitness_ruleset.csv", true);
        //worldIC.SetupPopulationFile("./output/pop_ic.csv", true);
        //worldRuleset.SetupPopulationFile("./output/pop_ruleset.csv", true);

        for (size_t id = 0; id < worldIC.GetSize(); id++) {
            std::cout << "IC ID: " << id << std::endl;
             print_fun_classic_ic(worldIC.GetOrg(id), std::cout);
        }
        for (size_t id = 0; id < worldRuleset.GetSize(); id++) {
            std::cout << "R ID: " << id << std::endl;
            print_fun_classic_ruleset(worldRuleset.GetOrg(id), std::cout);
        }
        
        std::multimap<double, size_t> fit_map;
        double cur_fit = 0;

        fit_map.clear();
        for (size_t id = 0; id < worldIC.GetSize(); id++) {
            cur_fit = worldIC.GetCache(id);
            fit_map.insert( std::make_pair(cur_fit, id) );
        }
        std::fstream icFP;
        std::fstream rulesetFP;
        std::ostringstream oss;
        //Main Loop
        for(size_t i = 0; i < numGens; i++){
            worldRuleset.ClearCache();
            worldIC.ClearCache();
            std::cout << "Generation #" << i << std::endl;
            if(eliteCount > 0 && eliteCopies > 0)
                EliteSelect(worldIC, eliteCount, eliteCopies);
            TournamentSelect(worldIC, tourneySize, tourneyCount);
            fit_map.clear();
            for (size_t id = 0; id < worldIC.GetSize(); id++) {
                cur_fit = worldIC.GetCache(id);
                fit_map.insert( std::make_pair(cur_fit, id) );
            }
            SetMaxFitIC(fit_map.rbegin()->second);
            if(eliteCount > 0 && eliteCopies > 0)
                EliteSelect(worldRuleset, eliteCount, eliteCopies);
            TournamentSelect(worldRuleset, tourneySize, tourneyCount);
            for (size_t id = 0; id < worldIC.GetSize(); id++) {
                std::cout << "IC ID: " << id << " - > ";
                std::cout << worldIC.GetCache(id) << std::endl;
            }
            for (size_t id = 0; id < worldRuleset.GetSize(); id++) {
                std::cout << "R ID: " << id << " - > ";
                std::cout << worldRuleset.GetCache(id) << std::endl;
            }
            //Save off members of both populations to check convergence
            oss.str("");
            oss << "./output/IC_Gen_" << i << ".txt";
            icFP.open(oss.str(), std::ios::out | std::ios::trunc);
            for (size_t id = 0; id < worldIC.GetSize(); id++) {
                icFP << "IC ID: " << id << std::endl;
                icFP << "Fitness: " << worldIC.GetCache(id) << std::endl;
                print_fun_classic_ic(worldIC.GetOrg(id), icFP);
                icFP << std::endl;
            }
            icFP.close(); 
            
            oss.str("");
            oss << "./output/Ruleset_Gen_" << i << ".txt";
            rulesetFP.open(oss.str(), std::ios::out | std::ios::trunc);
            for (size_t id = 0; id < worldRuleset.GetSize(); id++) {
                rulesetFP << "Ruleset ID: " << id << std::endl;
                rulesetFP << "Fitness: " << worldRuleset.GetCache(id) << std::endl;
                print_fun_classic_ruleset(worldRuleset.GetOrg(id), rulesetFP);
                rulesetFP << std::endl;
            }
            rulesetFP.close(); 
            worldIC.DoMutations();
            worldIC.Update();
            worldRuleset.DoMutations();
            worldRuleset.Update();
        }

        // Last generation image generation
        std::cout << "Last generation finished! Finishing up..." << std::endl; 
        for (size_t id = 0; id < worldIC.GetSize(); id++) {
            double cur_fit = fit_fun_ic(worldIC.GetOrg(id));
            fit_map.insert( std::make_pair(cur_fit, id) );
        }

        //icFP.open("./output/finalICGen.txt", std::ios::out | std::ios::trunc);
        //for (size_t id = 0; id < worldIC.GetSize(); id++) {
        //    icFP << "IC ID: " << id << std::endl;
        //    print_fun_classic_ic(worldIC.GetOrg(id), icFP);
        //    icFP << std::endl;
        //}
        //icFP.close(); 
        //
        //rulesetFP.open("./output/finalRulesetGen.txt", std::ios::out | std::ios::trunc);
        //for (size_t id = 0; id < worldRuleset.GetSize(); id++) {
        //    rulesetFP << "Ruleset ID: " << id << std::endl;
        //    print_fun_classic_ruleset(worldRuleset.GetOrg(id), rulesetFP);
        //    rulesetFP << std::endl;
        //}
        //rulesetFP.close(); 
        /*
        auto viz = GetVisualizer(800,600);
        int sub_x = GetConfig().Fetch<int>("SUB_X");
        int sub_y = GetConfig().Fetch<int>("SUB_Y");
        int sub_w = GetConfig().Fetch<int>("SUB_WIDTH");
        auto m = fit_map.rbegin();
        std::cout << "Generating images..." << std::endl;
        std::ostringstream os;
        for(size_t i = 0; i < fit_map.size(); i++){
            ca.Reset(); 
            std::vector<unsigned char> tmp = BitstringToVec(worldIC.GetOrg(m->second), 18);
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
        std::vector<unsigned char> tmp = BitstringToVec(worldIC.GetOrg(fit_map.rbegin()->second), 18);
        ca.AddSubsurface(tmp, sub_x, sub_y, sub_w);
        viz->SaveImage("./output/Best.png");
        viz->Run();
        viz->LoadImage("./output/Best.png");
        viz->Run(); 
        */
    }
    else{ //Viz only
        std::cout << "VIZ_ONLY flag set. Only running visualization of ./output/Best.png" <<std::endl;
        auto viz = GetVisualizer(800,600);
        viz->LoadImage("./output/Best.png");
        viz->Run(); 
    }
   
    //Finished! 
    std::cout << "Done!" << std::endl;
}
