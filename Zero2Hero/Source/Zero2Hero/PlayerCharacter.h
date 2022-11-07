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
#include "Damageable.h"
#include "Projectile.h"
#include "DialogueSystem.h"
#include "Camera.h"
#include "Math/UnrealMathUtility.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ZERO2HERO_API APlayerCharacter : public ADamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

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
	UPROPERTY()
		ACamera* CameraFollowPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
		float CameraSensitivity = 0.2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
		TSubclassOf<ACamera> CameraClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
		float ClampVerticalUp = 80.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
		float ClampVerticalDown = -80.0f;
	UPROPERTY()
		UCapsuleComponent* CapCollider;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	//	USpringArmComponent* springArm;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	//	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Weapons")
		TArray<TSubclassOf<ARangedWeapon>> RangedWeapons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Weapons")
		ARangedWeapon* CurrentRangedWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Weapons")
		int currentWeapon = 0;
	UPROPERTY(BlueprintReadWrite)
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
	UPROPERTY()
		float AttackAnimTimer = 0.0f;
	UPROPERTY()
		TArray<ADamageable*> EnemiesInRange;

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
		float dashTime = 0.3f;
	UPROPERTY()
		float currentDashTime = 0.0f;
	UPROPERTY(BlueprintReadWrite, Category = "Dash Settings")
		bool isDashing;
	UPROPERTY()
		bool hasDashedInAir = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Settings")
		float speedAfterDash = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Settings")
		float dashPushDown = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Settings")
		float dashGroundedCheck = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Settings")
		float dashFriction = 1.0f;
	UPROPERTY()
		float normalFriction;
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
	UPROPERTY()
		ADialogueSystem* DialogueSystem;
	UPROPERTY(EditAnywhere, Category = "Dialogue Settings")
		TSubclassOf<ADialogueSystem> DialogueSystemClass;
  UPROPERTY()
		UCharacterMovementComponent* characterMovementComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Run Settings")
		bool canWallRun = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Run Settings")
		float minDistToWallRun = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Run Settings")
		float wallRunSpeed = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Run Settings")
		float wallRunGravity = 0.3f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Run Settings")
		float wallJumpUpwardsVelocity = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Run Settings")
		float wallJumpSidewaysVelocity = 500.0f;
	UPROPERTY()
		bool isWallRunning = false;
	UPROPERTY()
		float startingGravityScale;
	UPROPERTY()
		float startingTurnSpeed;
	UPROPERTY()
		float startingAirControl;
	UPROPERTY()
		FRotator initialRotSpeed;
	UPROPERTY()
		int latestWallRunDir = 0;

	UPROPERTY()
		bool isWallJumping = false;
	UPROPERTY()
		bool hasWallJumped = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Jump Settings")
		float minDistToWallJump = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Jump Settings")
		float wallJumpGravity = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Jump Settings")
		float wallJumpBackwardsVelocity = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Jump Settings")
		float wallJumpTime = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Jump Settings")
		float wallJumpGroundedCheck = 400.0f;
	UPROPERTY()
		float currentWallJumpTime = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
		TSubclassOf<AActor> AmmoDropBP;

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
	void AddEnemyInRange(ADamageable* newEnemy);
	void DeleteEnemyInRange(ADamageable* oldEnemy);
	void RangedAttack();
	void RangedAttackEnd();

	void HookShot();
	void GrappleTo();

	void BeginCrouch();
	void EndCrouch();

	virtual void Landed(const FHitResult& Hit) override;
	UFUNCTION(BlueprintImplementableEvent)
		void PlayerLanded();
	virtual void Jump() override;

	
	void DoubleJump();
	UFUNCTION(BlueprintImplementableEvent)
		void StartDoubleJump();

	void Dash();
	void GroundPound();
	bool isGrounded();

	void UpDownCheck(float amount);
	void LeftRightCheck(float amount);

	UFUNCTION()
		void ComboDamage();

	UFUNCTION()
		void SecondaryAttack();

	UFUNCTION()
		void Dialogue();

	UFUNCTION(BlueprintCallable)
		void ChangeToWeapon1();
	UFUNCTION(BlueprintCallable)
		void ChangeToWeapon2();
	UFUNCTION(BlueprintCallable)
		void ChangeToWeapon3();
	UFUNCTION(BlueprintCallable)
		void ChangeToWeapon4();
	UFUNCTION(BlueprintImplementableEvent)
		void SwitchWeapon();

	UFUNCTION()
		void WallRunCheck();
	UFUNCTION()
		void WallRun(int dir, FHitResult result);
	UFUNCTION()
		void StopWallRun();
	UFUNCTION(BlueprintImplementableEvent)
		void StartWallRun();

	UFUNCTION()
		void WalljumpCheck();
	UFUNCTION()
		void WallJump(FHitResult result);
	UFUNCTION()
		void StopWallJump();
	UFUNCTION(BlueprintImplementableEvent)
		void StartWallJump();

	UFUNCTION()
		void DropExcessAmmo();

	UFUNCTION(BlueprintImplementableEvent)
		void GrappleEnd();
};
