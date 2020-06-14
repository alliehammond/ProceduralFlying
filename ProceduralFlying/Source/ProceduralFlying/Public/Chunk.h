#pragma once

#include <vector>
#include "Math/Vector.h"
#include "GameFramework/Actor.h"


class Chunk
{
public:
    Chunk(int chunkSize, FVector centerPos, int chXCoord, int chYCoord); //Create an empty chunk
    void addBlock(AActor *newBlock);
    void deleteChunk();

    std::vector<AActor *> blocks;
    std::vector<AActor *> snowBuildups;
    //Unreal unit position
    FVector centerPosition;
    //Discrete chunk position
    int chunkXCoord;
    int chunkYCoord;
    AActor *snowBlockInstance = 0;
    AActor *snowBuildupInstance = 0;
};
