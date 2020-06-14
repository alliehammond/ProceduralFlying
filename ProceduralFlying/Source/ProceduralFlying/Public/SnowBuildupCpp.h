#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SnowBuildupCpp.generated.h"

UCLASS()
class PROCEDURALFLYING_API ASnowBuildupCpp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnowBuildupCpp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
