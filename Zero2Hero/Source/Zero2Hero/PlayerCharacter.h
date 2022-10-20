// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "RangedWeapon.h"
#include "GrapplingHook.h"
#include "HookPoint.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ZERO2HERO_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		int Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		int Ammo;
	UPROPERTY()
		int MaxHealth;
	UPROPERTY()
		int MaxAmmo;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
		float CameraSensitivity = 0.2;
	UPROPERTY()
		UCapsuleComponent* CapCollider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
		USpringArmComponent* springArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
		UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Weapons")
		TArray<TSubclassOf<ARangedWeapon>> RangedWeapons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Weapons")
		ARangedWeapon* CurrentRangedWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Weapons")
		int currentWeapon = 0;
	UPROPERTY()
		TArray<ARangedWeapon*> allRangedWeapons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grappling Hook")
		TSubclassOf<AGrapplingHook> Grappling;
	UPROPERTY()
		bool HasHookShot = true;
	UPROPERTY()
		AGrapplingHook* GrapplingHook;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grappling Hook")
		float GrapplingSpeed;
	UPROPERTY()
		float CurrentDistanceGrapple = 0.0f;
	UPROPERTY()
		bool Hooked;
	UPROPERTY()
		FVector DirectionGrapple;
	UPROPERTY()
		UStaticMeshComponent* ConeSight;
	UPROPERTY(EditAnywhere, Category = "Grappling Hook")
		TSubclassOf<AHookPoint> HookPoints;

	UPROPERTY(EditAnywhere)
		USphereComponent* MeleeCollider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Attack Settings")
		int MeleeDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Attack Settings")
		float MeleeAttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Attack Settings")
		float MeleeAttackCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Settings")
		bool canDoubleJump = true;
	UPROPERTY()
		int doubleJumpCount = 0;
	UPROPERTY()
		float currentDashCooldown = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Settings")
		float doubleJumpHeight = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Settings")
		float doubleJumpThrust = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Settings")
		float dashVelocity = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Settings")
		float dashCooldown = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Settings")
		float dashPushDown = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Settings")
		float dashGroundedCheck = 30.0f;
	UPROPERTY()
		bool hasDashed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground Pound Settings")
		float GroundPoundMinDist = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground Pound Settings")
		float GroundPoundForce = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground Pound Settings")
		float GroundPoundRadius = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground Pound Settings")
		int GroundPoundDamage = 1;
	UPROPERTY()
		bool hasGroundPounded = false;
	UPROPERTY()
		bool upDownPressed = false;
	UPROPERTY()
		bool leftRightPressed = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void cameraVertical(float amount);
	void cameraHorizontal(float amount);

	void MoveLeftRight(float speed);
	void MoveUpDown(float speed);

	void MeleeAttack();
	void RangedAttack();
	void RangedAttackEnd();

	void HookShot();
	void GrappleTo();

	void BeginCrouch();
	void EndCrouch();

	virtual void Landed(const FHitResult& Hit) override;
	virtual void Jump() override;

	void DoubleJump();

	void Dash();
	void GroundPound();

	void UpDownCheck(float amount);
	void LeftRightCheck(float amount);

	UFUNCTION(BlueprintCallable)
		int GetHealth();
	UFUNCTION(BlueprintCallable)
		int GetMaxHealth();
	UFUNCTION(BlueprintCallable)
		void IncreaseHealth(int amount);
	UFUNCTION(BlueprintCallable)
		void DecreaseHealth(int amount);

	UFUNCTION(BlueprintCallable)
		int GetAmmo();
	UFUNCTION(BlueprintCallable)
		int GetMaxAmmo();
	UFUNCTION(BlueprintCallable)
		void IncreaseAmmo(int amount);
	UFUNCTION(BlueprintCallable)
		void DecreaseAmmo(int amount);
	UFUNCTION(BlueprintCallable)
		void ChangeToWeapon1();
	UFUNCTION(BlueprintCallable)
		void ChangeToWeapon2();
	UFUNCTION(BlueprintCallable)
		void ChangeToWeapon3();
	UFUNCTION(BlueprintCallable)
		void ChangeToWeapon4();
};
