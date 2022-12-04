// Fill out your copyright notice in the Description page of Project Settings.


#include "Reanimator.h"

// Sets default values
AReanimator::AReanimator()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ReviveRadius = CreateDefaultSubobject<USphereComponent>(TEXT("Revive Radius"));
	ReviveRadius->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AReanimator::BeginPlay()
{
	Super::BeginPlay();
	
	ReviveRadius->OnComponentBeginOverlap.AddDynamic(this, &AReanimator::OnOverlapInRange);
	ReviveRadius->OnComponentEndOverlap.AddDynamic(this, &AReanimator::OnEndOverlapInRange);

	Radius = ReviveRadius->GetScaledSphereRadius();
}

// Called every frame
void AReanimator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->InputEnabled())
	{
		if (isReviving)
		{
			TimerToRevive += DeltaTime;

			if (TimerToRevive >= TimeToRevive)
			{
				TimerToRevive = 0;
				ReanimateEnemy();
			}
		}
		else
		{
			FindNewTarget();

			ReviveRadius->SetSphereRadius(0);
		}
	}

}

// Called to bind functionality to input
void AReanimator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AReanimator::OnDeath()
{
	if (ToRevive != nullptr)
	{
		ToRevive->SetBeingRevived(false);
		ToRevive->GetMesh()->SetAnimation(nullptr);
	}
}

float AReanimator::GetRunAwayDistance()
{
	return RunAwayDistance;
}

void AReanimator::ReanimateEnemy()
{
	ToRevive->SetHealth(ToRevive->GetMaxHealth());
	ToRevive->SetIsDead(false);
	ToRevive->SetBeingRevived(false);
	ToRevive->GetMesh()->SetAnimation(nullptr);

	if (ReanimationVFX != nullptr)
	{
		ToRevive->GetNiagaraComp()->SetAsset(nullptr);
	}

	isReviving = false;
	ToRevive = nullptr;

	if (BBC != nullptr)
	{
		BBC->SetValueAsBool("IsReviving", false);
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Reanimated"));
}

void AReanimator::FindNewTarget()
{
	ReviveRadius->SetSphereRadius(Radius);

	if (InRangeEnemies.Num() != 0)
	{
		for (AEnemy* enemy : InRangeEnemies)
		{
			if (enemy == this)
			{
				continue;
			}

			if (enemy != nullptr)
			{
				if (enemy->GetIsDead())
				{
					ToRevive = enemy;
					enemy->SetBeingRevived(true);
					isReviving = true;

					ToRevive->GetMesh()->SetAnimation(nullptr);

					if (BBC != nullptr)
					{
						BBC->SetValueAsBool("IsReviving", true);
						BBC->SetValueAsVector("ToRevive", enemy->GetActorLocation());
					}

					if (ReanimationVFX != nullptr)
					{
						enemy->GetNiagaraComp()->SetAsset(ReanimationVFX);
					}

					//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("New Target Selected"));

					break;
				}
			}
		}
	}
}

void AReanimator::OnOverlapInRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Overlap"));

	if (OtherActor == this)
	{
		return;
	}

	if (OtherActor == nullptr)
	{
		return;
	}

	if (OtherComp == nullptr)
	{
		return;
	}

	if (!OtherActor->ActorHasTag("Enemy"))
	{
		return;
	}

	if (OtherComp->ComponentHasTag("Ignore"))
	{
		return;
	}

	AEnemy* enemy = Cast<AEnemy>(OtherActor);

	if (!InRangeEnemies.Contains(enemy))
	{
		InRangeEnemies.Add(enemy);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, OtherActor->GetFName().ToString());
	}
}

void AReanimator::OnEndOverlapInRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == this)
	{
		return;
	}

	if (OtherActor == nullptr)
	{
		return;
	}

	if (OtherComp == nullptr)
	{
		return;
	}

	if (!OtherActor->ActorHasTag("Enemy"))
	{
		return;
	}

	if (OtherComp->ComponentHasTag("Ignore"))
	{
		return;
	}

	InRangeEnemies.Remove(Cast<AEnemy>(OtherActor));
}

