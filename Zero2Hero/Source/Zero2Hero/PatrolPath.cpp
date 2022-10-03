// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPath.h"
#include "Components/SplineComponent.h"

// Sets default values
APatrolPath::APatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	path = CreateDefaultSubobject<USplineComponent>(TEXT("Path"));
	path->SetupAttachment(RootComponent);
	path->bDrawDebug = true;

}

// Called when the game starts or when spawned
void APatrolPath::BeginPlay()
{
	Super::BeginPlay();
	getSplinePoints();
	
}

void APatrolPath::getSplinePoints()
{
	for (int i = 0; i <= path->GetNumberOfSplinePoints(); i++)
	{
		Locations.Add(path->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
	}
}

