// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerRadius = CreateDefaultSubobject<USphereComponent>(TEXT("Player Radius"));
	PlayerRadius->SetupAttachment(GetRootComponent());

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Shield VFX Component"));
	NiagaraComp->SetupAttachment(GetRootComponent());

	//BBC = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
	//BTC = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behaviour Component"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//AIPC->ConfigureSense(*SightConfig);

	NiagaraComp = FindComponentByClass<UNiagaraComponent>();

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

	if (GetController())
	{
		Cast<AAIController>(GetController())->RunBehaviorTree(BT);
		isRunning = true;
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!isRunning)
	{
		if (GetController())
		{
			Cast<AAIController>(GetController())->RunBehaviorTree(BT);
			isRunning = true;
		}
	}

	if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) == nullptr)
	{
		return;
	}

	if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->InputEnabled())
	{
		if (Disenage)
		{
			DisenageTimer += DeltaTime;

			if (DisenageTimer >= DisenageFor)
			{
				Disenage = false;
				DisenageTimer = 0;
			}
		}

		if (BBC != nullptr)
		{
			BBC->SetValueAsBool("isDead", isDead);
		}


		if (Health <= 0)
		{
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
	}
	else
	{
		if (BBC != nullptr)
		{
			BBC->SetValueAsBool("isDead", true);
			
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

bool AEnemy::GetDisenage()
{
	return Disenage;
}

bool AEnemy::GetCanSee()
{
	return CanSee;
}

void AEnemy::OnTargetDetected(AActor* actor, FAIStimulus stimulus)
{
	if (actor->ActorHasTag("Player"))
	{
		if (!Disenage)
		{
			CanSee = stimulus.WasSuccessfullySensed();
		}
		else
		{
			CanSee = false;
		}

		if (CanSee && ShouldReset && !Disenage)
		{
			StartingAggroLocation = GetActorLocation();
			ShouldReset = false;
		}
		else
		{
			ShouldReset = true;
		}

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
					//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Out Range"));
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

UNiagaraComponent* AEnemy::GetNiagaraComp()
{
	return NiagaraComp;
}

float AEnemy::GetMovementSpeed()
{
	return MovementSpeed;
}

bool AEnemy::GetShouldMove()
{
	return ShouldMove;
}

bool AEnemy::GetZMoveToAtStart()
{
	return ZMoveAtStart;
}

float AEnemy::GetDistanceFromGround()
{
	return DistanceFromGround;
}

void AEnemy::SetZMoveAtStart(bool newMoveAtStart)
{
	ZMoveAtStart = newMoveAtStart;
}

void AEnemy::SetStartZ(float newZ)
{
	DistanceFromGround = newZ;
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
							GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Unshield Enemy"));
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


