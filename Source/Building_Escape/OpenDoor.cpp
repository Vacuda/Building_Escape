// Copyright Vacuda 2020


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Math/Rotator.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h"
#include "WeightCheck.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	//set initial position, set target position
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	DoorAngleChange += InitialYaw;

	// //null check, error message
	// if(!PressurePlate){
	// 	UE_LOG(LogTemp, Error, TEXT("Actor has the open door component, but no pressureplate set."));
	// }	
}

void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(TotalMassOfActors()>=MassToOpenDoor){
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else{
		//check if within door close delay
		float CurrentTime = GetWorld()->GetTimeSeconds();
		if(CurrentTime - DoorLastOpened > DoorCloseDelay){
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	//door rotation
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, DoorAngleChange, DeltaTime, DoorOpenSpeed);

	//rotation fix
	if(CurrentRotation.Yaw>180.f){
		CurrentRotation.Yaw-=360.f; 
		DoorAngleChange-=360.f;
	}

	//set new rotation
	GetOwner()->SetActorRotation(CurrentRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	//door rotation
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, InitialYaw, DeltaTime, DoorCloseSpeed);

	//rotation fix
	if(CurrentRotation.Yaw>180.f){
		CurrentRotation.Yaw-=360.f;
		DoorAngleChange-=360.f;
	}

	//set new rotation
	GetOwner()->SetActorRotation(CurrentRotation);
}

float const UOpenDoor::TotalMassOfActors(){
	float TotalMass = 0.f;

	//find all overlapping actors
	TArray<AActor*> OverlappingActorsArr;
	PressurePlate->GetOverlappingActors(OUT OverlappingActorsArr);

	//add masses in the object array
	for(AActor* Actor:OverlappingActorsArr){
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}