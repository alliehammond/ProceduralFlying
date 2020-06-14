// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdController.h"

// Sets default values
ABirdController::ABirdController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABirdController::BeginPlay()
{
	Super::BeginPlay();
	
    currentSpeed = maxSpeed / 2.0f;
    curMaxSpeed = maxSpeed;
    curPitch = 0.0f;
}

void ABirdController::moveForward(float val)
{
    float curHeight = GetActorLocation().Z;
    if(val > 0.0f)
    {
        curMaxSpeed = maxSpeed * forwardMultiplier;
    }
    else
    if(curHeight < maxHeight && val < 0.0f)
    {
        curMaxSpeed = maxSpeed * backwardMultiplier;
    }
    else
    {
        curMaxSpeed = maxSpeed;
    }
}

void ABirdController::moveLeft(float val)
{
    FRotator curRot = GetActorRotation();
    curRot.Add(0.0f, -val * turnSpeed * prevDt, 0.0f);
    SetActorRotation(curRot);
}

// Called every frame
void ABirdController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    prevDt = DeltaTime;

    //Update current speed
    if(currentSpeed < curMaxSpeed)
    {
        currentSpeed += DeltaTime * accelSpeed;
    }
    if(currentSpeed > curMaxSpeed)
    {
        currentSpeed -= DeltaTime * decelSpeed;
    }

    //Move forwards in direction player is facing 
    FVector curPos = GetActorLocation();
    FVector curForward = GetActorRotation().Vector();
    FVector newPos = curPos + currentSpeed * DeltaTime * curForward;
    SetActorLocation(newPos, true);

    //Adjust pitch based on whether traveling up or down
    if(maxSpeed == curMaxSpeed)//Traveling level
    {
        if(curPitch < -DeltaTime * pitchRate - 0.1f)
        {
            curPitch += DeltaTime * pitchRate;
        }
        else
        if (curPitch > DeltaTime * pitchRate + 0.1f)
        {
            curPitch -= DeltaTime * pitchRate;
        }
        else
            curPitch = 0.0f;
    }
    else
    if(curMaxSpeed > maxSpeed + 0.1f)//Travelling down
    {
        curPitch -= DeltaTime * pitchRate * (1.0f - (-curPitch / maxDeltaPitch));
        if (curPitch < -maxDeltaPitch)curPitch = -maxDeltaPitch;
    }
    else
    if (curMaxSpeed < maxSpeed - 0.1f)//Travelling up
    {
        curPitch += DeltaTime * pitchRate * (1.0f - (curPitch / maxDeltaPitch));
        if (curPitch > maxDeltaPitch)curPitch = maxDeltaPitch;
    }
    FRotator curRot = GetActorRotation();
    curRot.Pitch = curPitch;
    SetActorRotation(curRot);
}

// Called to bind functionality to input
void ABirdController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("Forward", this, &ABirdController::moveForward);
    PlayerInputComponent->BindAxis("Left", this, &ABirdController::moveLeft);
}

