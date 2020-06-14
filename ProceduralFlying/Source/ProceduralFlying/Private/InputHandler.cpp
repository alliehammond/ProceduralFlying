// Fill out your copyright notice in the Description page of Project Settings.


#include "InputHandler.h"
#include "Components/InputComponent.h"
#include "TerrainGenerator.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
AInputHandler::AInputHandler()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInputHandler::BeginPlay()
{
	Super::BeginPlay();
    Super::SetupPlayerInputComponent(InputComponent);
	
    //InputComponent->BindAction("IncreaseRenderDistance", IE_Pressed, this, &AInputHandler::increaseRenderDistance);
}

//Deprecated since pause menu now increases render distance
void AInputHandler::increaseRenderDistance()
{
    //Try and find the terrain generator and call it's function
    UE_LOG(LogTemp, Display, TEXT("Increasing"));
}

// Called every frame
void AInputHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AInputHandler::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

