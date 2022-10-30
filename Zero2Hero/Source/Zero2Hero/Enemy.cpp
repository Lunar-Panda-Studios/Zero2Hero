// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerRadius = CreateDefaultSubobject<USphereComponent>(TEXT("Player Radius"));
	PlayerRadius->SetupAttachment(GetRootComponent());

	//AIPC = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPC"));
	//SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	//BBC = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
	//BTC = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behaviour Component"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//AIPC->ConfigureSense(*SightConfig);

	MainBody = FindComponentByClass<UBoxComponent>();
	AIPC = FindComponentByClass<UAIPerceptionComponent>();

	if (PlayerRadius != nullptr)
	{
		PlayerRadius->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);
		PlayerRadius->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnOverlapEnd);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No Player Radius Collider - Enemy"));
	}

	if (MainBody != nullptr)
	{
		MainBody->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnMainBodyHit);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No Main Body Collider (Box Collider) - Enemy"));
	}

	if (AIPC != nullptr)
	{
		AIPC->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemy::OnTargetDetected);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No AI Perception Component - Enemy"));
	}

	MovementComp = FindComponentByClass<UCharacterMovementComponent>();

	if (MovementComp)
	{
		MovementComp->MaxWalkSpeed = MovementSpeed;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No Movement Component - Enemy"));
	}

	Cast<AAIController>(GetController())->RunBehaviorTree(BT);

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OnFire || FlameThrowerDamageTimer != 0)
	{
		OnFire = false;
		if (FlameThrowerDamageTimer < FlameThrowerDamageTimerMax)
		{
			FlameThrowerDamageTimer += DeltaTime;
		}

		if (FlameThrowerDamageTimer >= FlameThrowerDamageTimerMax)
		{
			FlameThrowerDamageTimer = 0;
			DecreaseHealth(Damage);
		}
	}

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::Attack()
{
}

bool AEnemy::GetCanSee()
{
	return CanSee;
}

void AEnemy::OnTargetDetected(AActor* actor, FAIStimulus stimulus)
{
	if (actor->ActorHasTag("Player"))
	{
		CanSee = stimulus.WasSuccessfullySensed();

		if (BBC != nullptr)
		{
			BBC->SetValueAsBool("LineOfSight", CanSee);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No BBC"));
		}
	}
}

void AEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		if (OtherActor != this)
		{
			if (OtherActor->ActorHasTag("Player"))
			{
				if (OtherComp->ComponentHasTag("MainBody"))
				{
					InRange = true;
					//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("In Range"));
				}
			}
		}
	}
}

void AEnemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr)
	{
		if (OtherActor != this)
		{
			if (OtherActor->ActorHasTag("Player"))
			{
				if (OtherComp->ComponentHasTag("MainBody"))
				{
					InRange = false;
					/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Out Range"));*/
				}
			}
		}
	}
}

void AEnemy::SetOnFire(bool isOnFire)
{
	OnFire = isOnFire;
}

void AEnemy::SetFlameDamage(int amount)
{
	FlameDamage = amount;
}

bool AEnemy::GetInRange()
{
	return InRange;
}

bool AEnemy::GetIsShielded()
{
	return isShielded;
}

bool AEnemy::GetIsShieldReflect()
{
	return ReflectorShield;
}

TEnumAsByte<ElementType> AEnemy::GetShieldType()
{
	return CurrentShieldType;
}

void AEnemy::UnshieldEnemy()
{
	isShielded = false;
	PairedEnemy->PairedEnemy = nullptr;
	PairedEnemy = nullptr;
	CurrentShieldType = ElementType::None;
}

void AEnemy::SetEnemyPair(AEnemy* newPair)
{
	PairedEnemy = newPair;
}

void AEnemy::UnPair()
{
	PairedEnemy = nullptr;
}

void AEnemy::SetShieldType(TEnumAsByte<ElementType> newElement)
{
	CurrentShieldType = newElement;
	isShielded = true;
}

bool AEnemy::SetisReflectorShield()
{
	return false;
}

void AEnemy::OnMainBodyHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			if (OtherComp != nullptr)
			{
				if (OtherComp->ComponentHasTag("MeleeZone"))
				{
					if (!isShielded)
					{
						Cast<APlayerCharacter>(OtherActor)->AddEnemyInRange(this);

						ADamageable* otherDamageable = Cast<ADamageable>(OtherActor);
						DecreaseHealth(otherDamageable->GetDamage());

						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Damage Enemy from Melee Player"));
						//Destroy();
					}
					else
					{
						if (GetIsShieldReflect())
						{
							APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
							Player->AddEnemyInRange(this);

							ADamageable* otherDamageable = Cast<ADamageable>(OtherActor);
							DecreaseHealth(otherDamageable->GetDamage());

							Player->DecreaseHealth(otherDamageable->GetDamage());
						}
						else
						{
							UnshieldEnemy();
						}
					}
				}
			}
		}
	}
}

void AEnemy::OnMainBodyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr)
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			if (OtherComp != nullptr)
			{
				if (OtherComp->ComponentHasTag("MeleeZone"))
				{
					Cast<APlayerCharacter>(OtherActor)->DeleteEnemyInRange(this);
				}
			}
		}
	}
}

bool AEnemy::IsPositionReachable(FVector Position) 
{
	FVector PathStart = GetActorLocation();
	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), PathStart, Position, NULL);

	if (!NavPath)
	{
		return false;
	}

	return !NavPath->IsPartial();
}

void AEnemy::SetBehaviourTree(UBehaviorTreeComponent* BehaviourTree)
{
	BTC = BehaviourTree;
}

void AEnemy::SetBlackboard(UBlackboardComponent* Blackboard)
{
	BBC = Blackboard;
}


