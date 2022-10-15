// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "GrapplingHook.generated.h"

UCLASS()
class ZERO2HERO_API AGrapplingHook : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrapplingHook();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		float Range;

	UPROPERTY(EditAnywhere)
		USphereComponent* FireLocation;
	UPROPERTY(EditAnywhere)
		UCameraComponent* CameraComponent;
	UPROPERTY()
		FHitResult HookHit;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		bool Fire();
	UFUNCTION()
		FHitResult GetHit();
	UFUNCTION()
		void SetCamera(UCameraComponent* Camera);
	UFUNCTION()
		bool HookReturned();

};
