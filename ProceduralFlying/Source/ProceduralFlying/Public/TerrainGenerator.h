// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chunk.h"
#include "FastNoise.h"
#include <vector>
#include <utility>
#include "TerrainGenerator.generated.h"


UCLASS()
class PROCEDURALFLYING_API ATerrainGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATerrainGenerator();

    std::vector<Chunk *> loadedChunks;
    

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TerrainSettings)
        float blockSize;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TerrainSettings)
        float noiseExponentMountains;//Exponent for applying to noise for height generation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TerrainSettings)
        float noiseScaleMountains;//Noise scale for height generation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TerrainSettings)
        int chunkSize; //Blocks per chunk (chunkSize x chunkSize)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TerrainSettings)
        int chunkSpawnDistance; //How many chunks in each direction to spawn
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TerrainSettings)
        float maxTerrainHeight;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TerrainSettings)
        UClass *snowBlockBlueprint;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TerrainSettings)
        UClass *snowBuildupBP;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TerrainSettings)
        UClass* blockBlueprint;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TerrainSettings)
        float biomeNoiseScale;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TerrainSettings)
        float buildupScale;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    float blockMeshSize;

    //User input functions
    void increaseRenderDist();
    void decreaseRenderDist();

    std::vector<std::pair<int, int>> chunkLoadQueue;

private:
    void spawnChunk(int chunkXCoord, int chunkYCoord); //Chunk coordinates are discrete (chunk 1 next to chunk 2 etc.)
    float getNormalizedZValueFromNoise(float locX, float locY);

    //If this changes, the world must be regenerated
    float storedNoiseExponent;

    FastNoise perlinNoise;
    FastNoise perlinNoiseTemp;
    FastNoise perlinNoiseRainfall;
};
