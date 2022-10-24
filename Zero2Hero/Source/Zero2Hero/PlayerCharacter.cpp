// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "Kismet\KismetSystemLibrary.h"
#include "Enemy.h"

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

	startingGravityScale = GetCharacterMovement()->GravityScale;
	startingTurnSpeed = GetCharacterMovement()->RotationRate.Vector().Z;

	MeleePressMax = MeleeAttackSpeed + 0.2;
	this->GetCharacterMovement();
	TArray<UActorComponent*> Comps = GetComponentsByTag(UStaticMeshComponent::StaticClass(), TEXT("PlayerView"));
	ConeSight = Cast<UStaticMeshComponent>(Comps[0]);
	
	characterMovementComp = this->GetCharacterMovement();
	normalFriction = characterMovementComp->GroundFriction;
	
	//Adds Custom Collisions
	CapCollider->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnComponentBeginOverlap);
	//CapCollider->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnMyComponentEndOverlap);
	CapCollider->OnComponentHit.AddDynamic(this, &APlayerCharacter::OnComponentHit);
	MeleeCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//Spawn Params
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	//Gives Grapple Hook
	GrapplingHook = GetWorld()->SpawnActor<AGrapplingHook>(Grappling, GetActorLocation(), GetActorRotation(), spawnParams);
	GrapplingHook->SetCamera(Camera);
	GrapplingHook->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	//Giving Hook Points Grappling Hook
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), HookPoints, FoundActors);

	for (int i = 0; i < FoundActors.Num(); i++)
	{
		Cast<AHookPoint>(FoundActors[i])->SetGrapplingHook(GrapplingHook);
	}


	//Gives Weapon
	CurrentRangedWeapon = GetWorld()->SpawnActor<ARangedWeapon>(RangedWeapons[0], GetActorLocation(), GetActorRotation(), spawnParams);
	CurrentRangedWeapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	for (int i = 0; i < RangedWeapons.Num(); i++)
	{
		if (RangedWeapons[i] != nullptr)
		{
			//this may spawn the ice shotgun twice. gotta check this
			allRangedWeapons.Add(GetWorld()->SpawnActor<ARangedWeapon>(RangedWeapons[i], GetActorLocation(), GetActorRotation(), spawnParams));
			allRangedWeapons[i]->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}

	//Sets health
	MaxHealth = Health;
	MaxAmmo = Ammo;
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetVelocity().Z < 0)
	{
		WallRunCheck();
	}

	if (currentDashTime < dashTime && isDashing)
	{
		currentDashTime += DeltaTime;
	}
	else if (isDashing)
	{
		LaunchCharacter(GetActorForwardVector() * speedAfterDash, true, false);
		currentDashTime = 0;
		isDashing = false;
		characterMovementComp->GroundFriction = normalFriction;
	}
	currentDashCooldown += DeltaTime;
	

	if (IsAttacking)
	{
		MeleePressTimer += DeltaTime;

		if (MeleePressTimer >= MeleePressMax)
		{
			MeleeAttackNum = 0;
			MeleePressTimer = 0;
			IsAttacking = false;
			MeleeCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}

	if (MeleeCollider->IsCollisionEnabled())
	{
		MeleeTimer += DeltaTime;

		if (MeleeTimer >= MeleeAttackSpeed)
		{
			MeleeTimer = 0;
			MeleePressTimer = 0;
			MeleeCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
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

	if (Hooked)
	{
		if (!GrapplingHook->GetIsGrappling())
		{
			GrappleTo();
			Hooked = false;
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
	PlayerInputComponent->BindAxis(TEXT("MoveForwardBackward"), this, &APlayerCharacter::UpDownCheck);
	PlayerInputComponent->BindAxis(TEXT("MoveLeftRight"), this, &APlayerCharacter::MoveLeftRight);
	PlayerInputComponent->BindAxis(TEXT("MoveLeftRight"), this, &APlayerCharacter::LeftRightCheck);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayerCharacter::DoubleJump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("MeleeAttack"), IE_Pressed, this, &APlayerCharacter::MeleeAttack);
	PlayerInputComponent->BindAction(TEXT("MeleeAttack"), IE_Pressed, this, &APlayerCharacter::GroundPound);
	PlayerInputComponent->BindAction(TEXT("RangedAttack"), IE_Pressed, this, &APlayerCharacter::RangedAttack);
	PlayerInputComponent->BindAction(TEXT("RangedAttack"), IE_Released, this, &APlayerCharacter::RangedAttackEnd);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &APlayerCharacter::BeginCrouch);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &APlayerCharacter::EndCrouch);
	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &APlayerCharacter::Dash);
	PlayerInputComponent->BindAction(TEXT("GrapplingHook"), IE_Pressed, this, &APlayerCharacter::HookShot);
	PlayerInputComponent->BindAction(TEXT("Weapon1"), IE_Pressed, this, &APlayerCharacter::ChangeToWeapon1);
	PlayerInputComponent->BindAction(TEXT("Weapon2"), IE_Pressed, this, &APlayerCharacter::ChangeToWeapon2);
	PlayerInputComponent->BindAction(TEXT("Weapon3"), IE_Pressed, this, &APlayerCharacter::ChangeToWeapon3);
	PlayerInputComponent->BindAction(TEXT("Weapon4"), IE_Pressed, this, &APlayerCharacter::ChangeToWeapon4);
	


}

void APlayerCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (OtherActor != this)
	//{
	//	if (OverlappedComponent->ComponentHasTag("MeleeZone"))
	//	{
	//		//DecreaseHealth();
	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Take Damage"));
	//	}
	//}
}

void APlayerCharacter::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr)
	{
		if (OtherActor->ActorHasTag("GrapplePoint") || OtherActor->ActorHasTag("Hook"))
		{
			GrapplingHook->GetCable()->SetVisibility(false);
			GrapplingHook->GetInUseHook()->SetActorLocation(GrapplingHook->GetFireLocation()->GetComponentLocation());
			GrapplingHook->GetInUseHook()->Destroy();

		}
	}
}

void APlayerCharacter::cameraVertical(float amount)
{
	AddControllerPitchInput(amount * CameraSensitivity * GetWorld()->GetDeltaSeconds());

	//springArm->AddLocalRotation(FRotator(amount, 0, 0));
}

void APlayerCharacter::cameraHorizontal(float amount)
{
	AddControllerYawInput(amount * CameraSensitivity * GetWorld()->GetDeltaSeconds());

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

void APlayerCharacter::BeginCrouch() 
{
	Crouch(); 
	//will probably have to override this built in crouch function to make one that plays our crouch anim. Or I could put that here
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Crouching"));
}

void APlayerCharacter::EndCrouch()
{
	UnCrouch();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Uncrouching"));
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	PlayerLanded();
	doubleJumpCount = 0;
	if (isWallRunning)
	{
		StopWallRun();
	}
	if (hasGroundPounded)
	{
		TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
		traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		TArray<AActor*> ignoreActors;
		ignoreActors.Init(this, 1);
		TArray<AActor*> actors;
		UClass* seekClass = ACharacter::StaticClass();
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), GroundPoundRadius, traceObjectTypes, seekClass, ignoreActors, actors);
		hasGroundPounded = false;
		DrawDebugSphere(GetWorld(), GetActorLocation(), GroundPoundRadius, 12, FColor::Red, false, 1000.0f, ESceneDepthPriorityGroup::SDPG_Foreground, 5.0f);
		for (AActor* overlappedActor : actors) 
		{
			//for now it just spits out the overlapped actors into the log. When we implement health, it will be simple
			//to get the actors and call the function that removes health from them.
			UE_LOG(LogTemp, Log, TEXT("OverlappedActor: %s"), *overlappedActor->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, *overlappedActor->GetName());
		}
	}
}

void APlayerCharacter::UpDownCheck(float amount)
{
	if (amount != 0)
	{
		upDownPressed = true;
	}
	else
	{
		upDownPressed = false;
	}
}

void APlayerCharacter::LeftRightCheck(float amount)
{
	if (amount != 0)
	{
		leftRightPressed = true;
	}
	else
	{
		leftRightPressed = false;
	}
}

int APlayerCharacter::GetHealth()
{
	return Health;
}

int APlayerCharacter::GetMaxHealth()
{
	return MaxHealth;
}

void APlayerCharacter::IncreaseHealth(int amount)
{
	Health += amount;
}

void APlayerCharacter::DecreaseHealth(int amount)
{
	Health -= amount;
}

int APlayerCharacter::GetAmmo()
{
	return Ammo;
}

int APlayerCharacter::GetMaxAmmo()
{
	return MaxAmmo;
}

void APlayerCharacter::IncreaseAmmo(int amount)
{
	Ammo += amount;
}

void APlayerCharacter::DecreaseAmmo(int amount)
{
	Ammo -= amount;
}

void APlayerCharacter::Jump()
{
	Super::Jump();
	doubleJumpCount++;
}


void APlayerCharacter::DoubleJump()
{
	if (doubleJumpCount == 1 && canDoubleJump && !isWallRunning)
	{
		StartDoubleJump();
		FVector forwardDir = GetActorRotation().Vector();
		FVector dir = FVector(0, 0, doubleJumpHeight);
		if (upDownPressed || leftRightPressed)
		{
			dir = forwardDir * doubleJumpThrust + FVector(0, 0, doubleJumpHeight);
		}
		LaunchCharacter(dir, true, true);

		doubleJumpCount++;
	}
	else if (isWallRunning)
	{
		if (latestWallRunDir == -90)
		{
			FVector dir = FVector(0, wallJumpSidewaysVelocity * GetActorRightVector().Y, wallJumpUpwardsVelocity);
			LaunchCharacter(dir, false, true);
			
		}
		else
		{
			FVector dir = FVector(0, -wallJumpSidewaysVelocity * GetActorRightVector().Y, wallJumpUpwardsVelocity);
			LaunchCharacter(dir, false, true);
		}
		StopWallRun();
	}
}


void APlayerCharacter::Dash()
{
	if (currentDashCooldown >= dashCooldown)
	{
		FVector forwardDir = GetActorRotation().Vector();

		FVector LineTraceEnd = FVector(APlayerCharacter::GetActorLocation().X, APlayerCharacter::GetActorLocation().Y, APlayerCharacter::GetActorLocation().Z - dashGroundedCheck);
		FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(OUT Hit, GetActorLocation(), LineTraceEnd, ECollisionChannel::ECC_GameTraceChannel1, TraceParams, FCollisionResponseParams());
		DrawDebugLine(GetWorld(), GetActorLocation(), LineTraceEnd, FColor::Blue, false, 1.0f, 0, 5);
		FVector dir;
		if (!Hit.IsValidBlockingHit())
		{
			dir = forwardDir * dashVelocity / 2 + FVector(0, 0, -dashPushDown);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, TEXT("Hit"));
		}
		else
		{
			dir = forwardDir * dashVelocity;
		}
		LaunchCharacter(dir, true, true);
		currentDashCooldown = 0.0f;
		isDashing = true;
		characterMovementComp->GroundFriction = dashFriction;
	}
}

void APlayerCharacter::GroundPound()
{
	FVector LineTraceEnd = FVector(APlayerCharacter::GetActorLocation().X, APlayerCharacter::GetActorLocation().Y, APlayerCharacter::GetActorLocation().Z - GroundPoundMinDist);
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(OUT Hit, GetActorLocation(), LineTraceEnd, ECollisionChannel::ECC_GameTraceChannel1, TraceParams, FCollisionResponseParams());
	DrawDebugLine(GetWorld(), GetActorLocation(), LineTraceEnd, FColor::Blue, false, 5.0f, 0, 5);
	if(!Hit.IsValidBlockingHit())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, TEXT("GroundPound"));
		FVector downDir = FVector::DownVector;
		LaunchCharacter(downDir * GroundPoundForce, true, true);
		hasGroundPounded = true;
	}
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
		if (CurrentRangedWeapon->DecreaseCharge(CurrentRangedWeapon->GetUsage()))
		{
			CurrentRangedWeapon->PrimaryAttack();
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Ranged Attack"));
		}

	}
}

void APlayerCharacter::RangedAttackEnd()
{
	if (CurrentRangedWeapon != nullptr)
	{
		CurrentRangedWeapon->PrimaryAttackEnd();
	}
}

void APlayerCharacter::ChangeToWeapon1()
{
	if (currentWeapon != 0)
	{
		currentWeapon = 0;
		CurrentRangedWeapon = allRangedWeapons[currentWeapon];
		SwitchWeapon();
	}
	
}

void APlayerCharacter::ChangeToWeapon2()
{
	if (currentWeapon != 1)
	{
		currentWeapon = 1;
		CurrentRangedWeapon = allRangedWeapons[currentWeapon];
		SwitchWeapon();
	}
}

void APlayerCharacter::ChangeToWeapon3()
{
	if (currentWeapon != 2)
	{
		currentWeapon = 2;
		CurrentRangedWeapon = allRangedWeapons[currentWeapon];
		SwitchWeapon();
	}
}

void APlayerCharacter::ChangeToWeapon4()
{
	if (currentWeapon != 3)
	{
		currentWeapon = 3;
		CurrentRangedWeapon = allRangedWeapons[currentWeapon];
		SwitchWeapon();
	}
}

void APlayerCharacter::WallRunCheck()
{
	
	FVector startPos = GetActorLocation();
	FVector endPos = GetActorRightVector() * minDistToWall;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	TraceParams.AddIgnoredActor(this);
	FHitResult HitResultLeft;
	FHitResult HitResultRight;

	if (GetWorld()->LineTraceSingleByChannel(HitResultLeft, GetActorLocation(), GetActorLocation() + -endPos, ECollisionChannel::ECC_WorldStatic, TraceParams))
	{
		WallRun(-90, HitResultLeft); //left
	}
	else if (GetWorld()->LineTraceSingleByChannel(HitResultRight, GetActorLocation(), GetActorLocation() + endPos, ECollisionChannel::ECC_WorldStatic, TraceParams))
	{
		WallRun(90, HitResultRight); //right
	}
	else if(isWallRunning)
	{
		StopWallRun();
	}
}

void APlayerCharacter::WallRun(int dir, FHitResult result)
{
	UCharacterMovementComponent* moveComp = GetCharacterMovement();
	StartWallRun();
	isWallRunning = true;
	FRotator RotationOf90Degrees(0, dir, 0);
	FRotator LeftOrRightDirection = RotationOf90Degrees.RotateVector(result.Normal).Rotation();
	FRotator newRotation(0, LeftOrRightDirection.Yaw, 0);
	SetActorRotation(newRotation, ETeleportType::TeleportPhysics);
	LaunchCharacter(GetActorForwardVector() * wallRunSpeed, true, false);
	moveComp->GravityScale = 0.5f;
	latestWallRunDir = dir;
	moveComp->RotationRate.Vector() = FVector(moveComp->RotationRate.Vector().X, moveComp->RotationRate.Vector().Y, 0);
}

void APlayerCharacter::StopWallRun()
{
	UCharacterMovementComponent* moveComp = GetCharacterMovement();
	isWallRunning = false;
	moveComp->GravityScale = startingGravityScale;
	moveComp->RotationRate.Vector() = FVector(moveComp->RotationRate.Vector().X, moveComp->RotationRate.Vector().Y, startingTurnSpeed);
}

void APlayerCharacter::HookShot()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Pressed"));
	if (HasHookShot)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Hooked"));
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, GrapplingHook->GetGrapplePoint()->GetHumanReadableName());
		if(GrapplingHook->GetGrapplePoint() != nullptr)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Hook In Sight"));
			if (GrapplingHook->Fire())
			{
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, GrapplingHook->GetHit().ToString());

				//GrappleTo();
				Hooked = true;
				GrapplingHook->GetCable()->SetAttachEndTo(GrapplingHook->GetInUseHook(), GrapplingHook->GetInUseHook()->GetMainBody()->GetFName());
				GrapplingHook->GetCable()->SetVisibility(true);

			}
		}
	}
}

void APlayerCharacter::GrappleTo()
{
	DirectionGrapple = (GrapplingHook->GetHit().GetActor()->GetActorLocation() - GetActorLocation());
	LaunchCharacter(DirectionGrapple * GrapplingSpeed, true, true);
}

