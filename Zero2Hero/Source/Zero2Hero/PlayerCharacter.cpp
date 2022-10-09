// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapCollider = FindComponentByClass<UCapsuleComponent>();

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(springArm, USpringArmComponent::SocketName);

	MeleeCollider = CreateDefaultSubobject<USphereComponent>(TEXT("MeleeZone"));
	MeleeCollider->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	MeleePressMax = MeleeAttackSpeed + 0.2;

	CapCollider->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnComponentBeginOverlap);
	//CapCollider->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnMyComponentEndOverlap);

	MeleeCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	CurrentRangedWeapon = GetWorld()->SpawnActor<ARangedWeapon>(RangedWeapons[0], GetActorLocation(), GetActorRotation(), spawnParams);

	CurrentRangedWeapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsAttacking)
	{
		MeleePressTimer += DeltaTime;
	}

	if (MeleeCollider->IsActive())
	{
		MeleeTimer += DeltaTime;

		if (MeleeTimer >= MeleeAttackSpeed)
		{
			MeleeTimer = 0;
			MeleePressTimer = 0;
			MeleeCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}

	if (MeleePressTimer >= MeleePressMax)
	{
		MeleeAttackNum = 0;
		MeleePressTimer = 0;
		IsAttacking = false;
		MeleeCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (MeleeAttackNum >= 3)
	{
		MeleeCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CanAttack = false;
		IsAttacking = false;
		MeleeCooldownTimer += DeltaTime;
		if (MeleeCooldownTimer >= MeleeAttackCooldown)
		{
			MeleeCooldownTimer = 0;
			MeleeTimer = 0;
			CanAttack = true;
			MeleeAttackNum = 0;
		}
	}

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(TEXT("LookVertical"), this, &APlayerCharacter::cameraVertical);
	PlayerInputComponent->BindAxis(TEXT("LookHorizontal"), this, &APlayerCharacter::cameraHorizontal);
	PlayerInputComponent->BindAxis(TEXT("MoveForwardBackward"), this, &APlayerCharacter::MoveUpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveLeftRight"), this, &APlayerCharacter::MoveLeftRight);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("MeleeAttack"), IE_Pressed, this, &APlayerCharacter::MeleeAttack);
	PlayerInputComponent->BindAction(TEXT("RangedAttack"), IE_Pressed, this, &APlayerCharacter::RangedAttack);

}

void APlayerCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		if (OtherActor->ActorHasTag("MeleeZone"))
		{
			//DecreaseHealth();
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Take Damage"));
		}
	}
}

void APlayerCharacter::cameraVertical(float amount)
{
	AddControllerPitchInput(amount * TurnRate * GetWorld()->GetDeltaSeconds());

	//springArm->AddLocalRotation(FRotator(amount, 0, 0));
}

void APlayerCharacter::cameraHorizontal(float amount)
{
	AddControllerYawInput(amount * TurnRate * GetWorld()->GetDeltaSeconds());

	//springArm->AddLocalRotation(FRotator(0, amount, 0));
}

void APlayerCharacter::MoveLeftRight(float speed)
{
	FRotator Rotation(0, Controller->GetControlRotation().Yaw, 0);

	FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	this->AddMovementInput(Direction * speed);
}

void APlayerCharacter::MoveUpDown(float speed)
{
	FRotator Rotation(0, Controller->GetControlRotation().Yaw, 0);

	FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);

	this->AddMovementInput(Direction * speed);
}

void APlayerCharacter::MeleeAttack()
{
	if (CanAttack)
	{
		MeleeTimer = 0;
		MeleeCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MeleeAttackNum += 1;
		MeleePressTimer = 0;
		IsAttacking = true;
		

		switch (MeleeAttackNum)
		{
		case 1:
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack 1"));
			break;
		}
		case 2:
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack 2"));
			break;
		}
		case 3:
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack 3"));
			break;
		}
		default:
			break;
		} 
	}

}

void APlayerCharacter::RangedAttack()
{
	if (CurrentRangedWeapon != nullptr)
	{
		CurrentRangedWeapon->PrimaryAttack();
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Ranged Attack"));
	}
}

