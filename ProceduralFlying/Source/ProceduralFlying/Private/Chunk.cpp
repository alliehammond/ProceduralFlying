#include "Chunk.h"
#include "Components/InstancedStaticMeshComponent.h"

Chunk::Chunk(int chunkSize, FVector centerPos, int chXCoord, int chYCoord) : blocks(0), centerPosition(centerPos), chunkXCoord(chXCoord), chunkYCoord(chYCoord)
{ }

void Chunk::addBlock(AActor *newBlock)
{
    blocks.push_back(newBlock);
}

void Chunk::deleteChunk()
{
    if (snowBlockInstance)
    {
        snowBlockInstance->FindComponentByClass<UInstancedStaticMeshComponent>()->ClearInstances();
        snowBlockInstance->Destroy();
    }
    if (snowBuildupInstance)
    {
        snowBuildupInstance->FindComponentByClass<UInstancedStaticMeshComponent>()->ClearInstances();
        snowBuildupInstance->Destroy();
    }
}
