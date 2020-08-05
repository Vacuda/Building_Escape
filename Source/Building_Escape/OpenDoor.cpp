// Copyright Vacuda 2020


#include "OpenDoor.h"
#include "Kismet/GameplayStatics.h"  //for getallactorsofclass
#include "UObject/Class.h"
#include "Components/AudioComponent.h"  //for UAudioComponent
// #include "Engine/World.h"  //for getworld
#include "PressurePlate.h"  //for PressurePlate

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

	SetPressurePlateComponent();
	SetAudioComponent();
}

void UOpenDoor::SetPressurePlateComponent()
{
	PressurePlateComponent = DoorTriggerActor->FindComponentByClass<UPressurePlate>();

	//safety check
	if(!PressurePlateComponent){
		UE_LOG(LogTemp, Error, TEXT("No pressure plate component found on %s!"), *GetOwner()->GetName());
	}
}

void UOpenDoor::SetAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	//safety check
	if(!AudioComponent){
		UE_LOG(LogTemp, Error, TEXT("No audio component found on %s!"), *GetOwner()->GetName());
	}
}

void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//ptr check
	if(!PressurePlateComponent){return;}

	//get TotalMassOnPlate, ptr to the address of it on the Root Component
	float* TotalMassOnPlate = &(PressurePlateComponent->TotalMassOnPlate); 

	//Set Target_Yaw based on TotalMassOnPlate
	if(*TotalMassOnPlate<=0.f){
		Target_Yaw=Initial_Yaw;
	}
	if(*TotalMassOnPlate>0.f && *TotalMassOnPlate<25.f){
		Target_Yaw=Initial_Yaw;
	}
	if(*TotalMassOnPlate>=25.f && *TotalMassOnPlate<50.f){
		Target_Yaw=Initial_Yaw + (DoorAngleChange/8.f)*1.f;
	}
	if(*TotalMassOnPlate>=50.f && *TotalMassOnPlate<75.f){
		Target_Yaw=Initial_Yaw + (DoorAngleChange/8.f)*2.f;
	}
	if(*TotalMassOnPlate>=75.f && *TotalMassOnPlate<100.f){
		Target_Yaw=Initial_Yaw + (DoorAngleChange/8.f)*3.f;
	}
	if(*TotalMassOnPlate>=100.f){
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

	//possible fix rotation
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

void UOpenDoor::Shutdown(){

	bIsDoorDocked = true;

	//ptr check
	if(!AudioComponent){return;}
	
	AudioComponent->Play();

	//stop tick
	PrimaryComponentTick.SetTickFunctionEnable(false);

	//get rotation
	FRotator CurrentRotation = GetOwner()->GetActorRotation();

	//change to final rotation
	CurrentRotation.Yaw = Final_Yaw;

	//set new rotation
	GetOwner()->SetActorRotation(CurrentRotation);
}
