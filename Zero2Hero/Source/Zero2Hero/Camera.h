// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera.generated.h"

UCLASS()
class ZERO2HERO_API ACamera : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
		USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
		UCameraComponent* CameraComp;
	UPROPERTY()
		USphereComponent* Center;
	UPROPERTY()
		UStaticMeshComponent* ConeSight;

	UPROPERTY()
		AActor* Player;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		USpringArmComponent* GetSpringArm();
	UFUNCTION()
		UCameraComponent* GetCameraComp();
	UFUNCTION()
		USphereComponent* GetSphereComp();
	UFUNCTION()
		void SetPlayer(AActor* newPlayer);
	UFUNCTION()
		UStaticMeshComponent* GetConeSight();


};
