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

	//springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	//springArm->SetupAttachment(GetRootComponent());

	//Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//Camera->SetupAttachment(springArm, USpringArmComponent::SocketName);

	MeleeCollider = CreateDefaultSubobject<USphereComponent>(TEXT("MeleeZone"));
	MeleeCollider->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	startingGravityScale = GetCharacterMovement()->GravityScale;
	startingTurnSpeed = GetCharacterMovement()->RotationRate.Vector().Z;
	startingAirControl = GetCharacterMovement()->AirControl;
	initialRotSpeed = GetCharacterMovement()->RotationRate;

	//Spawn Params
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	CameraFollowPoint = GetWorld()->SpawnActor<ACamera>(CameraClass, GetActorLocation(), GetActorRotation(), spawnParams);
	CameraFollowPoint->SetPlayer(this);

	MeleePressMax = MeleeAttackSpeed + 0.2;
	this->GetCharacterMovement();

	ConeSight = CameraFollowPoint->GetConeSight();
	
	characterMovementComp = this->GetCharacterMovement();
	normalFriction = characterMovementComp->GroundFriction;
	
	//Adds Custom Collisions
	if (CapCollider != nullptr)
	{
		CapCollider->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnComponentBeginOverlap);
		//CapCollider->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnMyComponentEndOverlap);
		CapCollider->OnComponentHit.AddDynamic(this, &APlayerCharacter::OnComponentHit);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No Capule Collider"));
	}

	if (MeleeCollider != nullptr)
	{
		MeleeCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No Capsule Collider"));
	}

	//Gives Grapple Hook
	GrapplingHook = GetWorld()->SpawnActor<AGrapplingHook>(Grappling, GetActorLocation(), GetActorRotation(), spawnParams);
	//GrapplingHook->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, GrapplingHookSocket);
	//GrapplingHook->SetActorHiddenInGame(true);

	//Giving Hook Points Grappling Hook
	if (HookPoints != nullptr)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), HookPoints, FoundActors);

		for (int i = 0; i < FoundActors.Num(); i++)
		{
			Cast<AHookPoint>(FoundActors[i])->SetGrapplingHook(GrapplingHook);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No Hook Point set please check blueprint"));
	}

	for (int i = 0; i < RangedWeapons.Num(); i++)
	{
		if (RangedWeapons[i] != nullptr)
		{
			//this may spawn the ice shotgun twice. gotta check this
			allRangedWeapons.Add(GetWorld()->SpawnActor<ARangedWeapon>(RangedWeapons[i], GetActorLocation(), GetActorRotation(), spawnParams));
			//allRangedWeapons[i]->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RangedSocket);
			allRangedWeapons[i]->SetCamera(CameraFollowPoint);
			//allRangedWeapons[i]->SetActorHiddenInGame(true);
		}
	}
	CurrentRangedWeapon = allRangedWeapons[0];

	if (DialogueSystemClass != nullptr)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), DialogueSystemClass, FoundActors);

		if (FoundActors.Num() != 0)
		{
			DialogueSystem = Cast<ADialogueSystem>(FoundActors[0]);

			if (DialogueSystemClass == nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Cast Failed"));
			}
		}
	}
	if (Manager != nullptr)
	{
		Manager->SetCurrentCheckPoint(GetActorLocation());
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!hasWallJumped && !isWallJumping)
	{
		WalljumpCheck();
	}
	if (GetVelocity().Z < 0 && canWallRun && !isWallJumping && !hasWallJumped)
	{
		WallRunCheck();
	}


	if (isWallJumping)
	{
		currentWallJumpTime += DeltaTime;
		if (currentWallJumpTime > wallJumpTime)
		{
			hasWallJumped = true;
			StopWallJump();
			currentWallJumpTime = 0.0f;
		}
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

	if (MeleeCollider->IsCollisionEnabled())
	{
		//Turns off Collisions after attack end
		MeleeTimer += DeltaTime;

		if (MeleeTimer >= MeleeAttackSpeed)
		{
			MeleeTimer = 0;
			MeleePressTimer = 0;
			MeleeCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			EnemiesInRange.Empty();
			//AttackAnimTimer = 0;
		}
	}

	//Grapples to the hook point
	if (Hooked)
	{
		if (!GrapplingHook->GetIsGrappling())
		{
			GrappleTo();
			Hooked = false;
		}
	}

	if (DialogueSystem != nullptr)
	{
		if (DialogueSystem->GetUsingDialogue())
		{
			if (DialogueSystem->GetDialogueWidget()->IsVisible())
			{
				if (DialogueSystem->GetCurrentDialogue().DisableEverything)
				{
					Allow = false;
				}
				else
				{
					Allow = true;
				}
			}
			else
			{
				Allow = true;
			}
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No Dialogue System"));
	}

	if (isDead)
	{
		if (DropAmmo)
		{
			DropAmmo = false;
		}

		Allow = false;
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
	//Removed for now as we are cutting scope on the weapons
	/*PlayerInputComponent->BindAction(TEXT("SecondaryAttack"), IE_Pressed, this, &APlayerCharacter::SecondaryAttack);*/
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &APlayerCharacter::BeginCrouch);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &APlayerCharacter::EndCrouch);
	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &APlayerCharacter::Dash);
	PlayerInputComponent->BindAction(TEXT("GrapplingHook"), IE_Pressed, this, &APlayerCharacter::HookShot);
	PlayerInputComponent->BindAction(TEXT("Dialogue"), IE_Pressed, this, &APlayerCharacter::Dialogue);
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
	//		ADamageable* otherDamageable = Cast<ADamageable>(OtherActor);
	//		DecreaseHealth(otherDamageable->GetDamage());
	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Take Damage from Enemy"));
	//	}
	//}
}

void APlayerCharacter::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr)
	{
		if (OtherActor->ActorHasTag("GrapplePoint") || OtherActor->ActorHasTag("Hook"))
		{
			if (GrapplingHook->GetInUseHook() != nullptr)
			{
				GrapplingHook->GetCable()->SetVisibility(false);
				GrapplingHook->GetInUseHook()->SetActorLocation(GrapplingHook->GetFireLocation()->GetComponentLocation());
				GrapplingHook->GetInUseHook()->Destroy();
				GrappleEnd();
			}
		}
	}
}

void APlayerCharacter::cameraVertical(float amount)
{
	AddControllerPitchInput(amount * CameraSensitivity * GetWorld()->GetDeltaSeconds());

	if (CameraFollowPoint->GetSpringArm()->GetComponentRotation().Pitch + (amount * CameraSensitivity * -1) < ClampVerticalDown &&
		CameraFollowPoint->GetSpringArm()->GetComponentRotation().Pitch + (amount * CameraSensitivity * -1) > ClampVerticalUp)
	{
		CameraFollowPoint->GetSpringArm()->SetWorldRotation(FRotator(CameraFollowPoint->GetSpringArm()->GetComponentRotation().Pitch + (amount * CameraSensitivity * -1), CameraFollowPoint->GetSpringArm()->GetComponentRotation().Yaw, CameraFollowPoint->GetSpringArm()->GetComponentRotation().Roll));
	}
}

void APlayerCharacter::cameraHorizontal(float amount)
{
	AddControllerYawInput(amount * CameraSensitivity * GetWorld()->GetDeltaSeconds());
	CameraFollowPoint->GetSpringArm()->SetWorldRotation(FRotator(CameraFollowPoint->GetSpringArm()->GetComponentRotation().Pitch, CameraFollowPoint->GetSpringArm()->GetComponentRotation().Yaw + (amount * CameraSensitivity), CameraFollowPoint->GetSpringArm()->GetComponentRotation().Roll));
}

void APlayerCharacter::MoveLeftRight(float speed)
{
	if (Allow)
	{
		FRotator Rotation(0, CameraFollowPoint->GetSpringArm()->GetComponentRotation().Yaw, 0);

		FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

		this->AddMovementInput(Direction * speed);
	}
}

void APlayerCharacter::MoveUpDown(float speed)
{
	if (Allow)
	{
		FRotator Rotation(0, CameraFollowPoint->GetSpringArm()->GetComponentRotation().Yaw, 0);

		FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);

		this->AddMovementInput(Direction * speed);
	}
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
	hasDashedInAir = false;
	
	GetCharacterMovement()->RotationRate.Vector() = FVector(GetCharacterMovement()->RotationRate.Vector().X, GetCharacterMovement()->RotationRate.Vector().Y, startingTurnSpeed);
	if (hasWallJumped)
	{
		hasWallJumped = false;
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

		ADamageable* DamageableTarget;

		for (AActor* overlappedActor : actors) 
		{

			DamageableTarget = Cast<ADamageable>(overlappedActor);

			if (DamageableTarget->GetShieldType() != ElementType::None)
			{
				DamageableTarget->DecreaseHealth(GroundPoundDamage);

				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Damage Ground Pound"));

				//UE_LOG(LogTemp, Log, TEXT("OverlappedActor: %s"), *overlappedActor->GetName());
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, *overlappedActor->GetName());
			}
			else
			{
				UnshieldEnemy();
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Unshield Enemy"));
			}
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

void APlayerCharacter::Jump()
{
	if (Allow)
	{
		Super::Jump();
		doubleJumpCount++;
	}
}


void APlayerCharacter::DoubleJump()
{
	if (doubleJumpCount == 1 && canDoubleJump && !isWallRunning && !isWallJumping)
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
	else if (isWallJumping)
	{
		StopWallJump();
		FVector rot = -GetActorForwardVector();
		SetActorRotation(rot.Rotation());
		FVector dir = FVector(wallJumpBackwardsVelocity * GetActorForwardVector());
		dir.Z = wallJumpUpwardsVelocity;
		hasWallJumped = false;
		LaunchCharacter(dir, true, true);
	}
	else if (isWallRunning && !isWallJumping)
	{
		if (latestWallRunDir == -90)
		{
			FVector dir = FVector(wallJumpSidewaysVelocity * GetActorRightVector());
			dir.Z = wallJumpUpwardsVelocity;
			LaunchCharacter(dir, false, true);
			
		}
		else
		{
			FVector dir = FVector(-wallJumpSidewaysVelocity * GetActorRightVector());
			dir.Z = wallJumpUpwardsVelocity;
			LaunchCharacter(dir, false, true);
		}
		StopWallRun();
	}
}


void APlayerCharacter::Dash()
{
	if (Allow)
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
				if (hasDashedInAir)
				{
					return;
				}
				dir = forwardDir * dashVelocity / 2 + FVector(0, 0, -dashPushDown);
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, TEXT("Hit"));
				hasDashedInAir = true;
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
}

void APlayerCharacter::GroundPound()
{
	if (Allow)
	{
		if (!isGrounded())
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, TEXT("GroundPound"));
			FVector downDir = FVector::DownVector;
			LaunchCharacter(downDir * GroundPoundForce, true, true);
			hasGroundPounded = true;
		}
		
	}
}

bool APlayerCharacter::isGrounded()
{
	FVector LineTraceEnd = FVector(APlayerCharacter::GetActorLocation().X, APlayerCharacter::GetActorLocation().Y, APlayerCharacter::GetActorLocation().Z - GroundPoundMinDist);
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(OUT Hit, GetActorLocation(), LineTraceEnd, ECollisionChannel::ECC_GameTraceChannel1, TraceParams, FCollisionResponseParams());
	DrawDebugLine(GetWorld(), GetActorLocation(), LineTraceEnd, FColor::Blue, false, 5.0f, 0, 5);

	return Hit.IsValidBlockingHit();
}

void APlayerCharacter::MeleeAttack()
{ 
	if (Allow)
	{
		if (CanAttack && isGrounded())
		{
			MeleeTimer = 0;
			MeleeCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			//MeleeAttackNum += 1;
			MeleePressTimer = 0;
			IsAttacking = true;


			//switch (MeleeAttackNum)
			//{
			//case 1:
			//{
			//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack 1"));
			//	break;
			//}
			//case 2:
			//{
			//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack 2"));
			//	ComboDamage();
			//	break;
			//}
			//case 3:
			//{
			//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack 3"));
			//	ComboDamage();
			//	break;
			//}
			//default:
			//	break;
			//}
		}
	}

}

void APlayerCharacter::RangedAttack()
{
	if (Allow)
	{
		if (CurrentRangedWeapon != nullptr)
		{
			FRotator Rotator = FRotator(GetActorRotation().Pitch, CameraFollowPoint->GetSpringArm()->GetComponentRotation().Yaw, GetActorRotation().Roll);
			SetActorRotation(Rotator);
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

void APlayerCharacter::AddEnemyInRange(ADamageable* newEnemy)
{
	EnemiesInRange.Add(newEnemy);
}

void APlayerCharacter::ComboDamage()
{
	for (ADamageable* Damageable : EnemiesInRange)
	{
		Damageable->DecreaseHealth(Damage);
	}
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Damage Enemy from Melee Player"));
}

void APlayerCharacter::SecondaryAttack()
{
	//removing secondary attack for now as we are cutting scope
	/*if (Allow)
	{
		if (CurrentRangedWeapon != nullptr)
		{
			CurrentRangedWeapon->SecondaryAttack();
		}
	}*/
}

void APlayerCharacter::Dialogue()
{
	if (DialogueSystem != nullptr)
	{
		DialogueSystem->OnClick();
	}
}

void APlayerCharacter::ChangeToWeapon1()
{
	if (currentWeapon != 0 && Allow)
	{
		currentWeapon = 0;
		CurrentRangedWeapon = allRangedWeapons[currentWeapon];
		SwitchWeapon();
	}
	
}

void APlayerCharacter::ChangeToWeapon2()
{
	if (currentWeapon != 1 && Allow)
	{
		currentWeapon = 1;
		CurrentRangedWeapon = allRangedWeapons[currentWeapon];
		SwitchWeapon();
	}
}

void APlayerCharacter::ChangeToWeapon3()
{
	if (currentWeapon != 2 && Allow)
	{
		currentWeapon = 2;
		CurrentRangedWeapon = allRangedWeapons[currentWeapon];
		SwitchWeapon();
	}
}

void APlayerCharacter::ChangeToWeapon4()
{
	if (currentWeapon != 3 && Allow)
	{
		currentWeapon = 3;
		CurrentRangedWeapon = allRangedWeapons[currentWeapon];
		SwitchWeapon();
	}
}

void APlayerCharacter::WallRunCheck()
{
	FVector startPos = GetActorLocation();
	FVector endPos = GetActorRightVector() * minDistToWallRun;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	TraceParams.AddIgnoredActor(this);
	FHitResult HitResultLeft;
	FHitResult HitResultRight;

	if (GetWorld()->LineTraceSingleByChannel(HitResultLeft, GetActorLocation(), GetActorLocation() + -endPos, ECollisionChannel::ECC_WorldStatic, TraceParams))
	{
		if (HitResultLeft.GetActor() != nullptr)
		{
			if (HitResultLeft.GetComponent()->ComponentHasTag(FName("Wall")))
			{
				WallRun(-90, HitResultLeft); //left
			}
		}
	}
	else if (GetWorld()->LineTraceSingleByChannel(HitResultRight, GetActorLocation(), GetActorLocation() + endPos, ECollisionChannel::ECC_WorldStatic, TraceParams))
	{
		if (HitResultRight.GetActor() != nullptr)
		{
			if (HitResultRight.GetComponent()->ComponentHasTag(FName("Wall")))
			{
				WallRun(90, HitResultRight); //right
			}
		}
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
	moveComp->GravityScale = wallRunGravity;
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

void APlayerCharacter::WalljumpCheck()
{
	FVector endPos = GetActorForwardVector() * minDistToWallJump;
	FVector downPos = -GetActorUpVector() * wallJumpGroundedCheck;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	TraceParams.AddIgnoredActor(this);
	FHitResult result;
	FHitResult downResult;
	if (!GetWorld()->LineTraceSingleByChannel(downResult, GetActorLocation(), GetActorLocation() + downPos, ECollisionChannel::ECC_WorldStatic, TraceParams))
	{
		if (GetWorld()->LineTraceSingleByChannel(result, GetActorLocation(), GetActorLocation() + endPos, ECollisionChannel::ECC_WorldStatic, TraceParams))
		{
			if (result.GetActor() != nullptr && !isWallJumping)
			{
				if (result.GetComponent()->ComponentHasTag(FName("Wall")))
				{
					WallJump(result);
				}
			}
		}
		else if (isWallJumping)
		{
			StopWallJump();
		}
	}
	else if (isWallJumping)
	{
		StopWallJump();
	}
	
}

void APlayerCharacter::WallJump(FHitResult result)
{	
	UCharacterMovementComponent* moveComp = GetCharacterMovement();
	moveComp->StopMovementKeepPathing();
	StartWallJump();
	isWallJumping = true;
	moveComp->GravityScale = wallJumpGravity;
	moveComp->RotationRate = FRotator::ZeroRotator;
	moveComp->AirControl = 0.0f;
}

void APlayerCharacter::StopWallJump()
{
	UCharacterMovementComponent* moveComp = GetCharacterMovement();
	moveComp->RotationRate = initialRotSpeed;	
	isWallJumping = false;
	moveComp->GravityScale = startingGravityScale;
	moveComp->AirControl = startingAirControl;
	currentWallJumpTime = 0;
}

void APlayerCharacter::HookShot()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Pressed"));
	if (HasHookShot)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Hooked"));
		if(GrapplingHook->GetGrapplePoint() != nullptr)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Hook In Sight"));
			if (GrapplingHook->Fire())
			{
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, GrapplingHook->GetHit().ToString());

				Hooked = true;
				if (GrapplingHook->GetCable() != nullptr)
				{
					GrapplingHook->GetCable()->SetAttachEndTo(GrapplingHook->GetInUseHook(), GrapplingHook->GetInUseHook()->GetMainBody()->GetFName());
					GrapplingHook->GetCable()->SetVisibility(true);
				}
			}
		}
	}
}

void APlayerCharacter::DeleteEnemyInRange(ADamageable* oldEnemy)
{
	EnemiesInRange.Remove(oldEnemy);
}

void APlayerCharacter::GrappleTo()
{
	DirectionGrapple = (GrapplingHook->GetHit().GetActor()->GetActorLocation() - GetActorLocation());

	LaunchCharacter(DirectionGrapple * GrapplingSpeed, true, true);
}

void APlayerCharacter::DropExcessAmmo()
{
	for(ARangedWeapon* Weapon:allRangedWeapons)
	{
		for (int i = 1; i < Weapon->GetAmmo(); i++)
		{
			//Drop ammo in random spots in radius

			//GetWorld()->SpawnActor<AActor>(AmmoDrop)
		}
	}
}

void APlayerCharacter::SetPlayerVisability(bool ShouldHide)
{
	SetActorHiddenInGame(ShouldHide);
	for (ARangedWeapon* Ranged : allRangedWeapons)
	{
		Ranged->SetActorHiddenInGame(ShouldHide);
	}

	if (GrapplingHook != nullptr)
	{
		GrapplingHook->SetActorHiddenInGame(ShouldHide);
	}

}

