// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthAmmoDrop.h"

// Sets default values
AHealthAmmoDrop::AHealthAmmoDrop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHealthAmmoDrop::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealthAmmoDrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealthAmmoDrop::SetAmmo(int ammoType, const int ammo)
{
	switch (ammoType)
	{
	case 0:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Ammo"));
		GetBlueAmmo(ammo);
		break;
	case 1:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Ammo"));
		GetGreenAmmo(ammo);
		break;
	case 2:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Ammo"));
		GetYellowAmmo(ammo);
		break;
	case 3:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Ammo"));
		GetRedAmmo(ammo);
		break;
	default:
		GetBlueAmmo(ammo);
		break;
	}
}


