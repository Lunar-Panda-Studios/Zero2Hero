// Fill out your copyright notice in the Description page of Project Settings.


#include "Buffer.h"

// Sets default values
ABuffer::ABuffer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TargetRange = CreateDefaultSubobject<USphereComponent>(TEXT("NewShieldTargetRange"));
	TargetRange->SetupAttachment(GetRootComponent());

	MeleeCollider = CreateDefaultSubobject<USphereComponent>(TEXT("MeleeCollider"));
	MeleeCollider->SetupAttachment(GetRootComponent());

	FireLocation = CreateDefaultSubobject<USphereComponent>(TEXT("FireLocation"));
	FireLocation->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ABuffer::BeginPlay()
{
	Super::BeginPlay();
	
	TargetRange->OnComponentBeginOverlap.AddDynamic(this, &ABuffer::OnBeginOverlapTargetRange);
	TargetRange->OnComponentEndOverlap.AddDynamic(this, &ABuffer::OnEndOverlapTargetRange);

	SphereRadius = TargetRange->GetScaledSphereRadius();
	TargetRange->SetSphereRadius(0);
}

// Called every frame
void ABuffer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) == nullptr)
	{
		return;
	}

	if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->InputEnabled())
	{
		TimerForEachType += DeltaTime;

		TargetRange->SetSphereRadius(SphereRadius);

		//if (PairedEnemy != nullptr)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, PairedEnemy->GetFName().ToString());
		//}

		if (TimerForEachType >= TimeForEachType)
		{
			switch (CurrentShieldType)
			{
			case 0:
			{
				CurrentShieldType = ElementType::Fire;
				if (PairedEnemy != nullptr)
				{
					PairedEnemy->SetShieldType(ElementType::Fire);
					if (ShieldVFX.Contains(ElementType::Fire))
					{
						Cast<AEnemy>(PairedEnemy)->GetNiagaraComp()->SetAsset(ShieldVFX[ElementType::Fire]);
					}
				}

				if (ShieldVFX.Contains(ElementType::Fire))
				{
					NiagaraComp->SetAsset(ShieldVFX[ElementType::Fire]);
				}

				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Fire"));
				break;
			}
			case 1:
			{
				CurrentShieldType = ElementType::Electric;
				if (PairedEnemy != nullptr)
				{
					PairedEnemy->SetShieldType(ElementType::Electric);
					if (ShieldVFX.Contains(ElementType::Electric))
					{
						Cast<AEnemy>(PairedEnemy)->GetNiagaraComp()->SetAsset(ShieldVFX[ElementType::Electric]);
					}
				}

				if (ShieldVFX.Contains(ElementType::Electric))
				{
					NiagaraComp->SetAsset(ShieldVFX[ElementType::Electric]);
				}

				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Shock"));
				break;
			}
			case 2:
			{
				CurrentShieldType = ElementType::Nature;
				if (PairedEnemy != nullptr)
				{
					PairedEnemy->SetShieldType(ElementType::Nature);
					if (ShieldVFX.Contains(ElementType::Nature))
					{
						Cast<AEnemy>(PairedEnemy)->GetNiagaraComp()->SetAsset(ShieldVFX[ElementType::Nature]);
					}
				}

				if (ShieldVFX.Contains(ElementType::Nature))
				{
					NiagaraComp->SetAsset(ShieldVFX[ElementType::Nature]);
				}

				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Nature"));
				break;
			}
			case 3:
			{
				CurrentShieldType = ElementType::Ice;
				if (PairedEnemy != nullptr)
				{
					PairedEnemy->SetShieldType(ElementType::Ice);
					if (ShieldVFX.Contains(ElementType::Ice))
					{
						Cast<AEnemy>(PairedEnemy)->GetNiagaraComp()->SetAsset(ShieldVFX[ElementType::Ice]);
					}
				}

				if (ShieldVFX.Contains(ElementType::Ice))
				{
					NiagaraComp->SetAsset(ShieldVFX[ElementType::Ice]);
				}

				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Ice"));
				break;
			}
			default:
			{
				NiagaraComp->SetAsset(nullptr);
				if (PairedEnemy != nullptr)
				{
					Cast<AEnemy>(PairedEnemy)->GetNiagaraComp()->SetAsset(nullptr);
				}
				break;
			}
			}

			TimerForEachType = 0;
		}

		if (!isPaired)
		{
			if (SeekNewTarget())
			{
				if (CanSee)
				{
					AttackSpeedTimer += DeltaTime;

					if (AttackSpeedTimer >= AttackSpeed)
					{
						RangedAttack();
						AttackSpeedTimer = 0;
					}
				}
			}
		}

		
	}
}

// Called to bind functionality to input
void ABuffer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ABuffer::SeekNewTarget()
{
	if (TargetsInRange.Num() != 0)
	{
		PairedEnemy = TargetsInRange[0];
		
		if (BBC != nullptr)
		{
			BBC->SetValueAsBool("IsPaired", true);
			BBC->SetValueAsObject("PairedEnemy", Cast<AActor>(PairedEnemy));
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemies Paired"));
			isPaired = true;
			isShielded = true;
			CurrentShieldType = ElementType::Fire;
			PairedEnemy->SetShieldType(ElementType::Fire);

			switch (CurrentShieldType)
			{
			case ElementType::Ice:
			{
				if (ShieldVFX.Contains(ElementType::Ice))
				{
					NiagaraComp->SetAsset(ShieldVFX[ElementType::Ice]);
					Cast<AEnemy>(PairedEnemy)->GetNiagaraComp()->SetAsset(ShieldVFX[ElementType::Ice]);
				}
			}
			case ElementType::Fire:
			{
				if (ShieldVFX.Contains(ElementType::Fire))
				{
					NiagaraComp->SetAsset(ShieldVFX[ElementType::Fire]);
					Cast<AEnemy>(PairedEnemy)->GetNiagaraComp()->SetAsset(ShieldVFX[ElementType::Fire]);
				}
			}
			case ElementType::Electric:
			{
				if (ShieldVFX.Contains(ElementType::Electric))
				{
					NiagaraComp->SetAsset(ShieldVFX[ElementType::Electric]);
					Cast<AEnemy>(PairedEnemy)->GetNiagaraComp()->SetAsset(ShieldVFX[ElementType::Electric]);
				}
			}
			case ElementType::Nature:
			{
				if (ShieldVFX.Contains(ElementType::Nature))
				{
					NiagaraComp->SetAsset(ShieldVFX[ElementType::Nature]);
					Cast<AEnemy>(PairedEnemy)->GetNiagaraComp()->SetAsset(ShieldVFX[ElementType::Nature]);
				}
			}
			default:
			{
				NiagaraComp->SetAsset(nullptr);
				Cast<AEnemy>(PairedEnemy)->GetNiagaraComp()->SetAsset(nullptr);
				break;
			}
			}

			if (isReflectorVariant)
			{
				SetisReflectorShield(true);
				PairedEnemy->SetisReflectorShield(true);
			}
			else
			{
				SetisReflectorShield(false);
				PairedEnemy->SetisReflectorShield(false);
			}

			return true;
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No BBC - Paired"));
		}

		return false;
	}
	else
	{
		isPaired = false;
		if (BBC != nullptr)
		{
			BBC->SetValueAsBool("IsPaired", false);
			BBC->SetValueAsObject("PairedEnemy", nullptr);
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No Paired"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No BBC"));
		}
		return true;
	}
}

void ABuffer::MeleeAttack(float DeltaTime)
{
	if (InRange && AttackSpeedTimer == 0)
	{
		OnAttack();
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

void ABuffer::RangedAttack()
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	UWorld* World = GEngine->GameViewport->GetWorld();
	AActor* Player = World->GetFirstPlayerController()->GetPawn();

	if (Player != nullptr)
	{
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation());
		AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileBP, FireLocation->GetComponentLocation(), Rotation, spawnParams);
		/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Spawned"));*/
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Player is Null"));
	}
}

void ABuffer::OnDeath()
{
	UnshieldEnemy();
}

void ABuffer::OnBeginOverlapTargetRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Overlap"));
	if (OtherActor != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Not null"));
		if (OtherActor->ActorHasTag("Enemy"))
		{
			if (OtherComp->ComponentHasTag("Ignore"))
			{
				return;
			}

			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy In Targetting Range"));
			ADamageable* EnteringEnemy = Cast<ADamageable>(OtherActor);

			if (EnteringEnemy != this && !EnteringEnemy->GetIsShielded() && !TargetsInRange.Contains(EnteringEnemy))
			{
				TargetsInRange.Add(EnteringEnemy);
			}
		}
	}
}

void ABuffer::OnEndOverlapTargetRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr)
	{
		if (OtherActor->ActorHasTag("Enemy"))
		{
			if (OtherComp->ComponentHasTag("Ignore"))
			{
				return;
			}

			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy Out of Targetting Range"));
			ADamageable* ExitingEnemy = Cast<ADamageable>(OtherActor);

			if (PairedEnemy == ExitingEnemy)
			{
				if (PairedEnemy != nullptr)
				{
					PairedEnemy->UnshieldEnemy();
				}
			}

			TargetsInRange.Remove(ExitingEnemy);
		}
	}
}

