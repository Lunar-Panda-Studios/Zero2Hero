// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "BounceProjectile.generated.h"

UCLASS()
class ZERO2HERO_API ABounceProjectile : public AProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABounceProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AProjectile> SplitInto;
	UPROPERTY(EditAnywhere)
		int NoSplitInto = 3;
	UPROPERTY(EditAnywhere)
		float SplitAngle = 30;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void Split(AActor* OtherActor);

	UFUNCTION()
		virtual void OnComponentHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
