// Fill out your copyright notice in the Description page of Project Settings.


#include "FireBomb.h"

// Sets default values
AFireBomb::AFireBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFireBomb::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFireBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

