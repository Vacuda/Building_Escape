// Copyright Vacuda 2020

#include "Pressure_Plate_Move.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UPressure_Plate_Move::UPressure_Plate_Move()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPressure_Plate_Move::BeginPlay()
{
	Super::BeginPlay();

	//set player as actor
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

	Initial_Z = GetOwner()->GetActorLocation().Z;
	Target_Z = Initial_Z - Plate_Depression_Amount;
	
}


// Called every frame
void UPressure_Plate_Move::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(PressurePlate->IsOverlappingActor(ActorThatOpens)){
		DepressPlate(DeltaTime);
	}
	else{
		RaisePlate(DeltaTime);
	}

}

void UPressure_Plate_Move::DepressPlate(float DeltaTime)
{
	FVector CurrentLocation = GetOwner()->GetActorLocation();

	CurrentLocation.Z = FMath::FInterpTo(CurrentLocation.Z, Target_Z, DeltaTime, Plate_Depression_Speed);

	GetOwner()->SetActorLocation(CurrentLocation);
}

void UPressure_Plate_Move::RaisePlate(float DeltaTime)
{
	FVector CurrentLocation = GetOwner()->GetActorLocation();

	CurrentLocation.Z = FMath::FInterpTo(CurrentLocation.Z, Initial_Z, DeltaTime, Plate_Raise_Speed);

	GetOwner()->SetActorLocation(CurrentLocation);
}

