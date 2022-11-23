// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoMakit.h"

// Sets default values
AAutoMakit::AAutoMakit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FireLocation = CreateDefaultSubobject<USphereComponent>(TEXT("Fire Location"));
	FireLocation->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AAutoMakit::BeginPlay()
{
	Super::BeginPlay();
	
	if (DistanceFromGround != 0)
	{
		FVector EndPoint = GetActorLocation();
		EndPoint.Z -= RaydownLength;
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);
		FHitResult Hit;

		GetWorld()->LineTraceSingleByChannel(OUT Hit, GetActorLocation(), EndPoint, ECollisionChannel::ECC_Visibility, TraceParams, FCollisionResponseParams());

		if (Hit.IsValidBlockingHit())
		{
			DistanceFromGround += Hit.ImpactPoint.Z;
			ZMoveAtStart = true;
		}

		if (BBC != nullptr)
		{
			BBC->SetValueAsBool("FlyToZ", ZMoveAtStart);
		}
	}
}

// Called every frame
void AAutoMakit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->InputEnabled())
	{
		if (ZMoveAtStart)
		{
			if (BBC != nullptr)
			{
				BBC->SetValueAsBool("FlyToZ", ZMoveAtStart);
			}

		}

		//	if (GetActorLocation().Z == DistanceFromGround)
		//	{
		//		ZMoveAtStart = false;
		//	}
		//}

		if (InRange)
		{
			AttackSpeedTimer += DeltaTime;

			if (AttackSpeedTimer >= AttackSpeed)
			{
				Attack();
				AttackSpeedTimer = 0;
			}
		}
	}
}

// Called to bind functionality to input
void AAutoMakit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAutoMakit::Attack()
{
	OnAttack();

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	UWorld* World = GEngine->GameViewport->GetWorld();
	AActor* Player = World->GetFirstPlayerController()->GetPawn();

	if (Player != nullptr)
	{
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation());
		AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileBP, FireLocation->GetComponentLocation(), Rotation, spawnParams);
		if (Projectile != nullptr)
		{
			Projectile->Damage = Damage;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Player is Null"));
	}
}

