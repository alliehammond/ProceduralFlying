#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/InputComponent.h"
#include "BirdController.generated.h"

UCLASS()
class PROCEDURALFLYING_API ABirdController : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABirdController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

    //Functions called when associated keys are pressed
    void moveForward(float val);
    void moveLeft(float val);

    float currentSpeed;
    float curMaxSpeed;
    float curPitch;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BirdSettings)
        float maxSpeed;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BirdSettings)
        float decelSpeed;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BirdSettings)
        float accelSpeed;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BirdSettings)
        float maxHeight;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BirdSettings)
        float forwardMultiplier;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BirdSettings)
        float backwardMultiplier;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BirdSettings)
        float turnSpeed;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BirdSettings)
        float maxDeltaPitch;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BirdSettings)
        float pitchRate;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    float prevDt;

};
