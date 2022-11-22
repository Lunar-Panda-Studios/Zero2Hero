// Fill out your copyright notice in the Description page of Project Settings.


#include "CannonFodder.h"

// Sets default values
ACannonFodder::ACannonFodder()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BlastRadius = CreateDefaultSubobject<USphereComponent>(TEXT("BlastRadius"));
	BlastRadius->SetupAttachment(GetRootComponent());

	MeleeCollider = CreateDefaultSubobject<USphereComponent>(TEXT("MeleeCollider"));
	MeleeCollider->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ACannonFodder::BeginPlay()
{
	Super::BeginPlay();

	if (BlastRadius != nullptr)
	{
		BlastRadius->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BlastRadius->OnComponentBeginOverlap.AddDynamic(this, &ACannonFodder::OnOverlapBeginExplode);
	}
	else
	{
		if (!CanExplode)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No Blast Radius - Cannon Fodder"));
		}
	}

	if (MeleeCollider != nullptr)
	{
		MeleeCollider->OnComponentBeginOverlap.AddDynamic(this, &ACannonFodder::OnOverlapMelee);
		MeleeCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		if (!CanExplode)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No Melee Collider - Cannon Fodder"));
		}
	}
}

// Called every frame
void ACannonFodder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->InputEnabled())
	{

		if (Health > 0)
		{
			if (!CanExplode)
			{
				if (InRange && AttackSpeedTimer == 0)
				{
					MeleeCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				}
				else
				{
					MeleeCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				}

				if (InRange)
				{
					AttackSpeedTimer += DeltaTime;

					if (AttackSpeedTimer >= AttackSpeed)
					{
						AttackSpeedTimer = 0;
					}
				}
			}
			else
			{
				if (InRange || shouldExplode)
				{
					shouldExplode = true;

					if (BBC != nullptr)
					{
						BBC->SetValueAsBool("Exploding", true);
					}

					Timer += DeltaTime;

					if (Timer >= TimeToExplode)
					{
						Explode();
					}
				}
				else
				{
					if (BBC != nullptr)
					{
						BBC->SetValueAsBool("Exploding", false);
					}
				}
			}
		}
	}
}

// Called to bind functionality to input
void ACannonFodder::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACannonFodder::Explode()
{
	BlastRadius->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//SetLifeSpan(AnimationTimer);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Knockback"));
	Destroy();

}

void ACannonFodder::OnOverlapBeginExplode(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		if (!OtherComp->ComponentHasTag("PlayerView"))
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
			Player->DecreaseHealth(Damage);

			FVector Direction = Player->GetActorLocation() - GetActorLocation();
			Direction.Normalize();


			FVector LineTraceEnd = FVector(Player->GetActorLocation().X, Player->GetActorLocation().Y, Player->GetActorLocation().Z - GroundLimit);
			FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

			FHitResult Hit;
			GetWorld()->LineTraceSingleByChannel(OUT Hit, GetActorLocation(), LineTraceEnd, ECollisionChannel::ECC_GameTraceChannel1, TraceParams, FCollisionResponseParams());
			//DrawDebugLine(GetWorld(), GetActorLocation(), LineTraceEnd, FColor::Blue, false, 1.0f, 0, 5);

			if (!Hit.IsValidBlockingHit())
			{
				Player->LaunchCharacter(Direction * (KnockbackSpeed/10), true, true);
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Not On Ground"));
			}
			else
			{
				Player->LaunchCharacter(Direction * KnockbackSpeed, true, true);
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("OnGround"));
			}
			
		}
	}
}

void ACannonFodder::OnOverlapMelee(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, OverlappedComponent->GetReadableName());
			if (OtherComp->ComponentHasTag("MainBody"))
			{

				ADamageable* otherDamageable = Cast<ADamageable>(OtherActor);
				otherDamageable->DecreaseHealth(GetDamage());
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Damage Player Melee"));
			}
		}
	}
}

