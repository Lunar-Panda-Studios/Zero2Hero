// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyWondering.generated.h"

UCLASS()
class ZERO2HERO_API AEnemyWondering : public AEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyWondering();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector StartLocation;

	UPROPERTY(EditAnywhere, Category = "Wondering")
		float XRadius;
	UPROPERTY(EditAnywhere, Category = "Wondering")
		float YRadius;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		float GetXRadius();
	UFUNCTION()
		float GetYRadius();
	UFUNCTION()
		FVector GetStartLocation();


};
