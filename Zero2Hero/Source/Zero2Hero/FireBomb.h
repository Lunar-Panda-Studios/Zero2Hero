// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "FireBomb.generated.h"

UCLASS()
class ZERO2HERO_API AFireBomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireBomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float radius = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float damage = 0;

	UPROPERTY(EditAnywhere)
		USphereComponent* sphere;

	UPROPERTY()
		TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	UPROPERTY()
		TArray<AActor*> ignoreActors;
	UPROPERTY()
		TArray<AActor*> actors;
	UPROPERTY()
		UClass* seekClass;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, class AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector normalImpulse, const FHitResult& Hit);

};
