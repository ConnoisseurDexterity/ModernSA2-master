#pragma once
// Add structs here 
#include "SA2ModLoader.h"
#include <string>
#include <vector>

//include <stdint.h>
//#include "ninja.h"
//#include "SA2Enums.h"

// extern LoopHead PathList_MetalH[];

FunctionPointer
(ObjectMaster, displayText, (int a1, const char* message, int displayTime, int language), 0x6B6E20);

/*/struct CameraInfo
{
    int field_0;
    int field_4;
    int field_8;
    char gapC[328];
    float field_154;
    float field_158;
    float field_15C;
    int field_160;
    char gap164[48];
    NJS_VECTOR position;
    Rotation rotation;
    char gap1AC[9000];
    int field_24D4;
}; */

//DataPointer(CameraInfo, CameraInfoPtr, 0x1DCFF40);//

/*/class IniReader {
public:
    int levelID;
    bool hasSimpleDeathPlane;
    float simpleDeathPlane;
    std::vector<std::vector<std::string>> importLevelQueries;
    IniReader(const char* path, const HelperFunctions& helperFunctions);
    void loadIniOptions();
    LoopHead** loadSplines();
    std::string getChaoGarden();
    std::string getLevelID();

private:
    const char* optionsPath;
    const char* gdPCPath;
    const HelperFunctions& helperFunctions;
    void printDebug(std::string message);
};*/



// rank quotes from Kell
DataPointer(char, LevelRank, 0x174B001);

struct ScoreWK
{
    char mode;
    char playerNum;
    char Rank;
    char Perfect;
    int Timer;
    int TimeBonus;
    int Score;
};
