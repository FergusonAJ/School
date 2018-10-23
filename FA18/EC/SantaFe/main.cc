// CSE 848 - Assigment 4 Task 2 (Santa Fe Ants)
// Austin Ferguson
// 10/16/2018

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

#include "./Config.h"
#include "./gp.h"




int main(int argc, char ** argv)
{
    srand(time(NULL));
    ConfigLoader config;
    ConfigInit(config); 
    config.Load("config.txt");

    GPManager gp;
    gp.Run();
    
    std::cout << "Done!" << std::endl;   
}
