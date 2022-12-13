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

	if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) == nullptr)
	{
		return;
	}

	if (isDead)
	{
		return;
	}

	if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->InputEnabled())
	{
		if (!HasSetStartLocation)
		{
			StartLocation = GetActorLocation();
			HasSetStartLocation = true;
		}
		else
		{
			if (CanSee)
			{
				if (RadiusCanFollow != 0)
				{
					if (BBC != nullptr)
					{
						FVector PatrolLocation = BBC->GetValueAsVector("PatrolLocation");
						PatrolLocation.Z = 0;
						FVector ActorLocation = GetActorLocation();
						ActorLocation.Z = 0;

						Mag = ((PatrolLocation - ActorLocation).Size());

						if (Mag > RadiusCanFollow)
						{
							Disenage = true;
							DisenageTimer = 0;
						}
					}
				}
			}
		}

		if (ZMoveAtStart)
		{
			if (BBC != nullptr)
			{
				BBC->SetValueAsBool("FlyToZ", ZMoveAtStart);
			}

		}

		if (InRange && !Disenage)
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

	FVector EndPoint = Player->GetActorLocation();
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);
	TraceParams.AddIgnoredActor(Player);
	FHitResult Hit;

	GetWorld()->LineTraceSingleByChannel(OUT Hit, GetActorLocation(), EndPoint, ECollisionChannel::ECC_Visibility, TraceParams, FCollisionResponseParams());

	if (!Hit.IsValidBlockingHit())
	{
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
}

