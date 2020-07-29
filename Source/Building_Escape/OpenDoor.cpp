// Copyright Vacuda 2020


#include "OpenDoor.h"
#include "WeightCheck.h"  //for WeightCheck

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	//set initial position, set target position
	Initial_Yaw = GetOwner()->GetActorRotation().Yaw;
	Target_Yaw = Initial_Yaw;
	Final_Yaw = DoorAngleChange + Initial_Yaw;	

	WeightCheckComponent = GetOwner()->FindComponentByClass<UWeightCheck>();
}

void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//get MassOnPlate
	float MassOnPlate = WeightCheckComponent->TotalMassOfActors();

	//Set Target_Yaw based on MassOnPlate
	if(MassOnPlate<=0.f){
		Target_Yaw=Initial_Yaw;
	}
	if(MassOnPlate>0.f && MassOnPlate<25.f){
		Target_Yaw=Initial_Yaw;
	}
	if(MassOnPlate>=25.f && MassOnPlate<50.f){
		Target_Yaw=Initial_Yaw + (DoorAngleChange/8.f)*1.f;
	}
	if(MassOnPlate>=50.f && MassOnPlate<75.f){
		Target_Yaw=Initial_Yaw + (DoorAngleChange/8.f)*2.f;
	}
	if(MassOnPlate>=75.f && MassOnPlate<100.f){
		Target_Yaw=Initial_Yaw + (DoorAngleChange/8.f)*3.f;
	}
	if(MassOnPlate>=100.f){
		Target_Yaw=Initial_Yaw + DoorAngleChange;
	}

	RotationCorrection(&Target_Yaw);

	MoveDoor(DeltaTime);
}

void UOpenDoor::MoveDoor(float DeltaTime)
{
	//get rotation
	FRotator CurrentRotation = GetOwner()->GetActorRotation();

	//change rotation according to Target
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, Target_Yaw, DeltaTime, DoorMoveSpeed);

	RotationCorrection(&CurrentRotation.Yaw);

	//set new rotation
	GetOwner()->SetActorRotation(CurrentRotation);
}

void UOpenDoor::RotationCorrection(float* Yaw)
{
	//used to fix rotation yaw if over 180 degrees
	if(*Yaw>180.f){
		*Yaw-=360.f; 
	}
}