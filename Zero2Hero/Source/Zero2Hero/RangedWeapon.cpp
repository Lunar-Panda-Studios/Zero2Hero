// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedWeapon.h"

// Sets default values
ARangedWeapon::ARangedWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//FireLocation = FindComponentByClass<USphereComponent>();

	//FireLocation = CreateDefaultSubobject<USphereComponent>(TEXT("Fire Location"));
	//FireLocation->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ARangedWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARangedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARangedWeapon::PrimaryAttack()
{

}

void ARangedWeapon::SecondaryAttack()
{
}

int ARangedWeapon::GetCharge()
{
	return Charge;
}

void ARangedWeapon::IncreaseCharge(int amount)
{
	Charge += amount;
}

bool ARangedWeapon::DecreaseCharge(int amount)
{
	if (Charge - amount < 0)
	{
		return false;
	}

	Charge -= amount;
	return true;
}

int ARangedWeapon::GetUsage()
{
	return ChargeUsage;
}

