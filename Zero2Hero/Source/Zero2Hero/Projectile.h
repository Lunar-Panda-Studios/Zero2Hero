// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Damageable.h"
#include "Projectile.generated.h"

UCLASS()
class ZERO2HERO_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	UPROPERTY()
		int Damage;
	UPROPERTY(EditAnywhere)
		UProjectileMovementComponent* ProjectMovementComp;
	UPROPERTY()
		UStaticMeshComponent* MainBody;
	UPROPERTY(EditAnywhere)
		bool isEnemyProjectile = false;
	UPROPERTY()
		TEnumAsByte<ElementType> ElementType = ElementType::None;
	UPROPERTY()
		bool HasDestruct = false;
	UPROPERTY()
		bool isHomingMissile = false;
	UPROPERTY(EditAnywhere, Category = "Projectile")
		float LifeSpan = 10;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		bool GetHasDestruct();

	UFUNCTION()
		virtual void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
