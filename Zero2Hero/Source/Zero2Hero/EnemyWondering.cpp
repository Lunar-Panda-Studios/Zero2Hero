// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWondering.h"

// Sets default values
AEnemyWondering::AEnemyWondering()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyWondering::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	
}

// Called every frame
void AEnemyWondering::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyWondering::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemyWondering::GetXRadius()
{
	return XRadius;
}

float AEnemyWondering::GetYRadius()
{
	return YRadius;
}

float AEnemyWondering::GetZRadius()
{
	return ZRadius;
}

FVector AEnemyWondering::GetStartLocation()
{
	return StartLocation;
}

