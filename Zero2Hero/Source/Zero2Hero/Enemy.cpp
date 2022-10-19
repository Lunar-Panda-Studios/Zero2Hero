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
	CanSee = stimulus.WasSuccessfullySensed();

	//if (CanSee)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Can See"));
	//}
	//else
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No See"));
	//}


}

void AEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		if (OtherActor != this)
		{
			if (OtherActor->ActorHasTag("Player"))
			{
				InRange = true;
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("In Range"));
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
				InRange = false;
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Out Range"));
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

void AEnemy::DecreaseHealth(int amount)
{
	Health -= amount;
}

void AEnemy::IncreaseHealth(int amount)
{
	Health += amount;
}

void AEnemy::OnMainBodyHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			if (OtherComp != nullptr)
			{
				if (OtherComp->ComponentHasTag("MeleeZone") || OtherComp->ComponentHasTag("Projectile"))
				{
					//DAMAGE SELF;

					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Damage Enemy"));
					Destroy();
				}
			}
		}
	}
}


