// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyChaseMelee.h"

// Sets default values
AEnemyChaseMelee::AEnemyChaseMelee()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyChaseMelee::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyChaseMelee::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyChaseMelee::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

