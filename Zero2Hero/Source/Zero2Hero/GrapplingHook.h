// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "CableComponent.h"
#include "Hook.h"
#include "Kismet/KismetMathLibrary.h"
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

	//UPROPERTY(EditAnywhere)
	//	float Range;

	UPROPERTY(EditAnywhere)
		USphereComponent* FireLocation;
	UPROPERTY(EditAnywhere)
		UCameraComponent* CameraComponent;
	UPROPERTY()
		FHitResult HookHit;
	//UPROPERTY(EditAnywhere, Category = "Grappling")
	//	float FiringRate = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Grappling")
		TSubclassOf<AHook> Hook;
	UPROPERTY(BlueprintReadWrite)
		bool isGrappling = false;
	UPROPERTY()
		bool canGrapple = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grappling")
		bool isEnabled = true;

	UPROPERTY()
		AActor* GrapplePoint = nullptr;
	UPROPERTY(EditAnywhere)
		UCableComponent* CableComp;
	UPROPERTY()
		AHook* InUseHook;
	UPROPERTY()
		float PreviousMag;
	UPROPERTY()
		bool EndGrapple = false;
	UPROPERTY(EditAnywhere, Category = "Grappling")
		float MagCheck = 50;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void Detach();
	UFUNCTION()
		bool Fire();
	UFUNCTION()
		FHitResult GetHit();
	UFUNCTION()
		void SetCamera(UCameraComponent* Camera);
	UFUNCTION()
		bool HookReturned();
	UFUNCTION()
		bool GetIsGrappling();
	UFUNCTION()
		void SetIsGrappling(bool newGrappling);
	UFUNCTION()
		UCableComponent* GetCable();
	UFUNCTION()
		USphereComponent* GetFireLocation();

	UFUNCTION()
		void SetGrapplePoint(AActor* NewPoint);
	UFUNCTION()
		AActor* GetGrapplePoint();
	UFUNCTION()
		AHook* GetInUseHook();
	UFUNCTION()
		bool GetEndGrapple();
	UFUNCTION()
		void SetEndGrapple(bool newGrapple);
	UFUNCTION()
		bool GetCanGrapple();

	UFUNCTION()
		void OnHit(AActor* OverlappedActor, AActor* OtherActor);

	//UFUNCTION()
	//	void CableOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
		void GrappleShoot();
	UFUNCTION(BlueprintImplementableEvent)
		void GrappleHit();
	
};
