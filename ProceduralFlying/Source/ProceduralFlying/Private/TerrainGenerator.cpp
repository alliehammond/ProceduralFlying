#include "TerrainGenerator.h"
#include "SnowBuildupCpp.h"

#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Math/Box.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InstancedStaticMeshComponent.h"

#include <stdio.h>
#include <time.h>

// Sets default values
ATerrainGenerator::ATerrainGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATerrainGenerator::BeginPlay()
{
	Super::BeginPlay();
    //Store noise exponent so if is modified the world can be regenerated
    storedNoiseExponent = noiseExponentMountains;
    time_t t;
    srand((unsigned) time(&t));
    perlinNoise.SetNoiseType(FastNoise::Perlin);

    perlinNoise.SetSeed(rand());

    //Calculate size of block mesh from its bounding box
    AActor *temp = GetWorld()->SpawnActor(blockBlueprint);
    FBox boundBox = temp->FindComponentByClass<UStaticMeshComponent>()->GetStaticMesh()->GetBoundingBox();
    blockMeshSize = boundBox.GetSize().X;
    temp->Destroy();

    //Force chunk spawn distance to be even
    if(chunkSpawnDistance % 2 == 1)++chunkSpawnDistance;
}

//Input functions
void ATerrainGenerator::increaseRenderDist()
{
    ++chunkSpawnDistance;
}
void ATerrainGenerator::decreaseRenderDist()
{
    --chunkSpawnDistance;
}


int floatToIntRound(float a)
{
    if(a < 0)
    {
        if(a - int(a) < -0.5f)return int(a) - 1;
        else
            return int(a);
    }
    if(a - int(a) > 0.5f)return int(a) + 1;
    return int(a);

}

float ATerrainGenerator::getNormalizedZValueFromNoise(float locX, float locY)
{
    float newBaseNoiseVal = perlinNoise.GetNoise(locX * noiseScaleMountains, locY * noiseScaleMountains);
    //Make noiseval between 0 and 1
    newBaseNoiseVal += 1.0f;
    newBaseNoiseVal /= 2.0f;
    float newFinalNoiseVal = pow(newBaseNoiseVal, noiseExponentMountains);
    float locZ = float(floatToIntRound(newFinalNoiseVal * maxTerrainHeight));
    return locZ;
}

void ATerrainGenerator::spawnChunk(int chunkXCoord, int chunkYCoord)
{
    float x = chunkXCoord * chunkSize * blockSize * blockMeshSize;
    float y = chunkYCoord * chunkSize * blockSize * blockMeshSize;

    Chunk *newChunk = new Chunk(chunkSize, FVector(x, y, 0.0f), chunkXCoord, chunkYCoord);
    loadedChunks.push_back(newChunk);


    //*************************************
    //Set up instancing for block meshes
    //*************************************

    //Create instances of the block blueprints
    FVector instanceLocation(0.0f, -999999.0f, 0.0f);
    FRotator rotation(0.0f, 0.0f, 0.0f);

    newChunk->snowBlockInstance = GetWorld()->SpawnActor(snowBlockBlueprint, &instanceLocation, &rotation);
    newChunk->snowBuildupInstance = GetWorld()->SpawnActor(snowBuildupBP, &instanceLocation, &rotation);

    //*************************************
    //Do the actual terrain generation
    //*************************************

    //Loop through all the blocks to spawn
    for(int i = -chunkSize / 2;i < chunkSize / 2; ++i)
    {
        for(int j = -chunkSize / 2;j < chunkSize / 2; ++j)
        {
            float locX = x + (float(i) + 0.5f) * blockSize * blockMeshSize;
            float locY = y + (float(j) + 0.5f) * blockSize * blockMeshSize;

            
            //Calculate z of block based on noise generator
            float baseNoiseVal = perlinNoise.GetNoise(locX * noiseScaleMountains, locY * noiseScaleMountains);
            //Make noiseval between 0 and 1
            baseNoiseVal += 1.0f;
            baseNoiseVal /= 2.0f;
            float finalNoiseVal = pow(baseNoiseVal, noiseExponentMountains);
            float locZ = float(floatToIntRound(finalNoiseVal * maxTerrainHeight)) * blockSize * blockMeshSize;

            //*************************************
            //Figure out how many blocks below need to be spawned
            //*************************************
            float tempLocZ = float(floatToIntRound(finalNoiseVal * maxTerrainHeight));

            //Calculate the maximum difference in height between current block and lower adjacent blocks
            float lowestNeighbor = 0;
            
            float locZLeft = getNormalizedZValueFromNoise((locX + 1 * blockSize * blockMeshSize), (locY + 0 * blockSize * blockMeshSize));
            if(tempLocZ - locZLeft > lowestNeighbor)lowestNeighbor = tempLocZ - locZLeft;
            float locZRight = getNormalizedZValueFromNoise((locX - 1 * blockSize * blockMeshSize), (locY + 0 * blockSize * blockMeshSize));
            if (tempLocZ - locZRight > lowestNeighbor)lowestNeighbor = tempLocZ - locZRight;
            float locZUp = getNormalizedZValueFromNoise((locX + 0 * blockSize * blockMeshSize), (locY + 1 * blockSize * blockMeshSize));
            if (tempLocZ - locZUp > lowestNeighbor)lowestNeighbor = tempLocZ - locZUp;
            float locZDown = getNormalizedZValueFromNoise((locX + 0 * blockSize * blockMeshSize), (locY - 1 * blockSize * blockMeshSize));
            if (tempLocZ - locZDown > lowestNeighbor)lowestNeighbor = tempLocZ - locZDown;
            

            FVector location(locX, locY, locZ);
            FRotator rotation(0.0f, 0.0f, 0.0f);
            FVector scale(blockSize, blockSize, blockSize);
            FTransform trans(rotation, location - instanceLocation, scale);
            FTransform snowTrans(rotation, location - instanceLocation, FVector(buildupScale * blockSize, buildupScale * blockSize, buildupScale * blockSize));

            newChunk->snowBuildupInstance->FindComponentByClass<UInstancedStaticMeshComponent>()->AddInstance(snowTrans);

            //If the lowest neighbor is a bigger than 1 block difference, spawn the necessary blocks
            for(int k = 1;k < lowestNeighbor; ++k)
            {
                FVector newLocation = location - FVector(0.0f, 0.0f, blockSize * blockMeshSize * k);
                FTransform newTrans(rotation, newLocation - instanceLocation, scale);

                newChunk->snowBlockInstance->FindComponentByClass<UInstancedStaticMeshComponent>()->AddInstance(newTrans);
            }

            newChunk->snowBlockInstance->FindComponentByClass<UInstancedStaticMeshComponent>()->AddInstance(trans);
        }
    }
}

// Called every frame
void ATerrainGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    //If true, delete all the loaded chunks and regenerate them
    bool regenerateAll = false;

    if(storedNoiseExponent != noiseExponentMountains)
    {
        regenerateAll  = true;
        storedNoiseExponent = noiseExponentMountains;
    }

    //Calculate center of current chunk
    FVector playerPos = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
    float totalChunkSize = chunkSize * blockSize * blockMeshSize;
    int chunkXCoord = floatToIntRound(playerPos.X / totalChunkSize);
    int chunkYCoord = floatToIntRound(playerPos.Y / totalChunkSize);

    //Loop through all possible chunks to spawn, if they aren't spawned, spawn them
    for(int i = chunkXCoord - chunkSpawnDistance; i <= chunkXCoord + chunkSpawnDistance; ++i)
    {
        for (int j = chunkYCoord - chunkSpawnDistance; j <= chunkYCoord + chunkSpawnDistance; ++j)
        {
            bool chunkFound = false;
            //Check if the current chunk exists
            for(int k = 0;k < loadedChunks.size(); ++k)
            {
                if(loadedChunks[k]->chunkXCoord == i && loadedChunks[k]->chunkYCoord == j)
                {
                    chunkFound = true;
                    break;
                }
            }
            //Check in the chunk load queue too
            if(!chunkFound)
            {
                for(int k = 0;k < chunkLoadQueue.size(); ++k)
                {
                    if(chunkLoadQueue[k].first == i && chunkLoadQueue[k].second == j)
                    {
                        chunkFound = true;
                        break;
                    }
                }
            }

            if(!chunkFound)
            {
                //Chunk wasn't found so add it to the spawn queue
                chunkLoadQueue.push_back(std::pair<int, int>(i, j));
            }
        }
    }

    //Load one chunk per frame
    if(chunkLoadQueue.size())
    {
        std::pair<int, int> newChunk;
        int closestDistSq = INT_MAX;
        //Iterator marking the chunk to be spawned (and deleted from queue)
        int j = -1;
        //Find closest chunk to player to spawn first
        for(int i = 0;i < chunkLoadQueue.size(); ++i)
        {
            int chunkDistSq = ((chunkLoadQueue[i].first - chunkXCoord) * (chunkLoadQueue[i].first - chunkXCoord) + 
                                (chunkLoadQueue[i].second - chunkYCoord) * (chunkLoadQueue[i].second - chunkYCoord));
            if(chunkDistSq < closestDistSq)
            {
                closestDistSq = chunkDistSq;
                newChunk.first = chunkLoadQueue[i].first;
                newChunk.second = chunkLoadQueue[i].second;
                j = i;
            }
        }
        
        spawnChunk(newChunk.first, newChunk.second);

        //Delete chunk from the queue
        chunkLoadQueue.erase(chunkLoadQueue.begin() + j);
    }
    
    //Loop through all the chunks and delete any that are out of range
    auto it = loadedChunks.begin();
    while(it != loadedChunks.end())
    {
        if(regenerateAll || ((*it)->chunkXCoord < chunkXCoord - chunkSpawnDistance || (*it)->chunkXCoord > chunkXCoord + chunkSpawnDistance
            || (*it)->chunkYCoord < chunkYCoord - chunkSpawnDistance || (*it)->chunkYCoord > chunkYCoord + chunkSpawnDistance))
        {
            (*it)->deleteChunk();
            it = loadedChunks.erase(it);
        }
        else
            ++it;
    }
}

