// Fill out your copyright notice in the Description page of Project Settings.


#include "BossFailSafeSpawn.h"

// Sets default values
ABossFailSafeSpawn::ABossFailSafeSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FailSafeLocation = CreateDefaultSubobject<USphereComponent>(TEXT("FailSafeLocation"));
	FailSafeLocation->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ABossFailSafeSpawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossFailSafeSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

USphereComponent* ABossFailSafeSpawn::GetFailSafeSphere()
{
	return FailSafeLocation;
}

