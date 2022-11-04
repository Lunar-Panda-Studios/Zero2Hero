// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SuctionGrenade.generated.h"

UCLASS()
class ZERO2HERO_API ASuctionGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASuctionGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		bool isSucking = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float succTime = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float succRadius = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float succSpeed = 50.0f;
	UPROPERTY()
		float currentSuccTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float explodeTime = 3.0f;
	UPROPERTY()
		float currentExplodeTime = 0.0f;

};
