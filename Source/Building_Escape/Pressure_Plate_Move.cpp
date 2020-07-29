// Copyright Vacuda 2020

#include "Pressure_Plate_Move.h"
#include "WeightCheck.h" //for WeightCheck

// Sets default values for this component's properties
UPressure_Plate_Move::UPressure_Plate_Move()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

}

void UPressure_Plate_Move::BeginPlay()
{
	Super::BeginPlay();

	//setup initial values
	Initial_Z = GetOwner()->GetActorLocation().Z;
	Final_Z = Initial_Z - Plate_Depression_Amount;

	//setup WeightCheckComponent
	WeightCheckComponent = GetOwner()->FindComponentByClass<UWeightCheck>();
}

void UPressure_Plate_Move::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//get MassOnPlate
	float MassOnPlate = WeightCheckComponent->TotalMassOfActors();

	//Set Target_Z based on MassOnPlate
	if(MassOnPlate<=0.f){
		Target_Z=Initial_Z;
	}
	if(MassOnPlate>0.f && MassOnPlate<25.f){
		Target_Z=Initial_Z;
	}
	if(MassOnPlate>=25.f && MassOnPlate<50.f){
		Target_Z=Initial_Z - (Plate_Depression_Amount/4.f)*1.f;
	}
	if(MassOnPlate>=50.f && MassOnPlate<75.f){
		Target_Z=Initial_Z - (Plate_Depression_Amount/4.f)*2.f;
	}
	if(MassOnPlate>=75.f && MassOnPlate<100.f){
		Target_Z=Initial_Z - (Plate_Depression_Amount/4.f)*3.f;
	}
	if(MassOnPlate>=100.f){
		Target_Z=Initial_Z - Plate_Depression_Amount;
	}
	MovePlate(DeltaTime);
}

void UPressure_Plate_Move::MovePlate(float DeltaTime)
{
	//get location
	FVector CurrentLocation = GetOwner()->GetActorLocation();

	//check
	if(CurrentLocation.Z <= Final_Z + 2.f){
		bIsStoneSet = true;

		LockDoorsOpen();

		//stop tick
		PrimaryComponentTick.SetTickFunctionEnable(false);
	}

	//change location based on Target_Z
	CurrentLocation.Z = FMath::FInterpTo(CurrentLocation.Z, Target_Z, DeltaTime, Plate_Move_Speed);

	//set location
	GetOwner()->SetActorLocation(CurrentLocation);
}

void UPressure_Plate_Move::LockDoorsOpen()
{







//find doors with tag PressurePlate_1
//for each one
//find OpenDoor Component
//set bIsDoorSet to true



}
