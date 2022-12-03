// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Hook.generated.h"

UCLASS()
class ZERO2HERO_API AHook : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHook();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		UProjectileMovementComponent* ProjectileMoveComp;
	UPROPERTY()
		UStaticMeshComponent* MainBody;
	UPROPERTY()
		bool HookAttached;
	UPROPERTY()
		FVector HookPointLocation;
	UPROPERTY()
		float PreviousMag;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SetHookPointLocation(FVector NewHookPointLocation);
	UFUNCTION()
		UProjectileMovementComponent* GetMoveComp();
	UFUNCTION()
		UStaticMeshComponent* GetMainBody();
	UFUNCTION()
		bool GetHookAttached();

	UFUNCTION()
		void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
