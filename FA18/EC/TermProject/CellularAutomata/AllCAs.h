#include "./Conway.h"
#include "./Wireworld.h"
#include "./Replicator.h"
#include "./Fredkin.h"
#include "./HighLife.h"

emp::vector<empCA::CAFunctionStruct<unsigned char>> automataStructs{
    conwayStruct,
    wireworldStruct,
    replicatorStruct,
    fredkinStruct,
    highlifeStruct
};
