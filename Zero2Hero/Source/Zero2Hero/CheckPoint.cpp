// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPoint.h"

// Sets default values
ACheckPoint::ACheckPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RespawnPoint = CreateDefaultSubobject<USphereComponent>(TEXT("RespawnPoint - Center"));
	//RespawnPoint->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
	
	Manager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));

	Mesh = FindComponentByClass<UStaticMeshComponent>();

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::ActiveCheckPoint);

	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void ACheckPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACheckPoint::ActiveCheckPoint(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		if (!OtherComp->ComponentHasTag("PlayerView"))
		{
			if (Manager == nullptr)
			{
				Manager = Cast<UGameManager>(GetWorld()->GetGameInstance());
			}
			
			Manager->SetCurrentCheckPoint(RespawnPoint->GetComponentLocation());

			for (ARangedWeapon* Weapon : Player->GetRangedWeapons())
			{
				if (Weapon->GetWeaponName() == "Ice")
				{
					Manager->SetIceAmmo(Weapon->GetAmmo());
				}
				else if (Weapon->GetWeaponName() == "Fire")
				{
					Manager->SetFireAmmo(Weapon->GetAmmo());
				}
				else if (Weapon->GetWeaponName() == "Electric")
				{
					Manager->SetElectricAmmo(Weapon->GetAmmo());
				}
				else if (Weapon->GetWeaponName() == "Nature")
				{
					Manager->SetNatureAmmo(Weapon->GetAmmo());
				}
			}
			Manager->SaveGame(SaveClass);
		}
	}
}

