// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "CheckPoint.generated.h"

UCLASS()
class ZERO2HERO_API ACheckPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		UGameManager* Manager;
	UPROPERTY(EditAnywhere)
		USphereComponent* RespawnPoint;
	UPROPERTY(EditAnywhere)
		int CheckPointNum = 0;
	UPROPERTY()
		UStaticMeshComponent* Mesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void ActiveCheckPoint(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
