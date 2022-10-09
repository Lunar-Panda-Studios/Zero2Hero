// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
//#include "RangedWeapon.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ZERO2HERO_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
		USpringArmComponent* springArm;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
		UCameraComponent* Camera;

	//UPROPERTY(EditAnywhere, Category = "Ranged Weapons")
	//	TArray<ARangedWeapon*> RangedWeapons;
	//UPROPERTY()
	//	ARangedWeapon* CurrentRangedWeapon;

	UPROPERTY(EditAnywhere)
		USphereComponent* MeleeCollider;

	UPROPERTY(EditAnywhere, Category = "Melee Attack Settings")
		int MeleeDamage;

	UPROPERTY(EditAnywhere, Category = "Melee Attack Settings")
		float MeleeAttackSpeed;

	UPROPERTY(EditAnywhere, Category = "Melee Attack Settings")
		float MeleeAttackCooldown;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY()
		float MeleePressTimer = 0;
	UPROPERTY()
		float MeleePressMax = 0;
	UPROPERTY()
		float MeleeTimer = 0;
	UPROPERTY()
		float MeleeCooldownTimer = 0;
	UPROPERTY()
		bool CanAttack = true;
	UPROPERTY()
		int MeleeAttackNum = 0;
	UPROPERTY()
		bool IsAttacking = false;
	UPROPERTY(EditAnywhere)
		float TurnRate = 0.2;
	UPROPERTY()
		UCapsuleComponent* CapCollider;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	void cameraVertical(float amount);
	void cameraHorizontal(float amount);

	void MoveLeftRight(float speed);
	void MoveUpDown(float speed);

	void MeleeAttack();
	void RangedAttack();

};
