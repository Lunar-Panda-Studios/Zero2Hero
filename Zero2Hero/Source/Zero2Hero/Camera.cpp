// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera.h"

// Sets default values
ACamera::ACamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACamera::BeginPlay()
{
	Super::BeginPlay();

	CameraComp = FindComponentByClass<UCameraComponent>();
	Center = FindComponentByClass<USphereComponent>();
	SpringArmComp = FindComponentByClass<USpringArmComponent>();

	if (SpringArmComp == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Null"));
	}

}

// Called every frame
void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Center->SetWorldLocation(Player->GetActorLocation());

}

USpringArmComponent* ACamera::GetSpringArm()
{
	return SpringArmComp;
}

UCameraComponent* ACamera::GetCameraComp()
{
	return CameraComp;
}

USphereComponent* ACamera::GetSphereComp()
{
	return Center;
}

void ACamera::SetPlayer(AActor* newPlayer)
{
	Player = newPlayer;
}

