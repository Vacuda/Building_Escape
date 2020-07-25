// Copyright Vacuda 2020


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Math/Rotator.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	//set initial position, set target position
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	DoorAngleChange += InitialYaw;

	//null check, error message
	if(!PressurePlate){
		UE_LOG(LogTemp, Error, TEXT("Actor has the open door component, but no pressureplate set."));
	}

	//set player as actor
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//pressure plate overlap check
	if(PressurePlate->IsOverlappingActor(ActorThatOpens)){
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
	//lerp door rotation
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
	//lerp door rotation
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
