// Copyright Vacuda 2020

#include "Pressure_Plate_Move.h"
#include "PressurePlate.h" //for PressurePlate

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

	//setup PressurePlateRootComponent
	PressurePlateRootComponent = GetOwner()->FindComponentByClass<UPressurePlate>();

	//safety check
	if(!PressurePlateRootComponent){
		UE_LOG(LogTemp, Error, TEXT("No pressure plate root component found"));
	}
}

void UPressure_Plate_Move::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//ptr check
	if(!PressurePlateRootComponent) {return;}

	//get TotalMassOnPlate, ptr to the address of it on the Root Component
	float* TotalMassOnPlate = &(PressurePlateRootComponent->TotalMassOnPlate);          

	//Set Target_Z based on TotalMassOnPlate
	if(*TotalMassOnPlate<=0.f){
		Target_Z=Initial_Z;
	}
	if(*TotalMassOnPlate>0.f && *TotalMassOnPlate<25.f){
		Target_Z=Initial_Z;
	}
	if(*TotalMassOnPlate>=25.f && *TotalMassOnPlate<50.f){
		Target_Z=Initial_Z - (Plate_Depression_Amount/4.f)*1.f;
	}
	if(*TotalMassOnPlate>=50.f && *TotalMassOnPlate<75.f){
		Target_Z=Initial_Z - (Plate_Depression_Amount/4.f)*2.f;
	}
	if(*TotalMassOnPlate>=75.f && *TotalMassOnPlate<100.f){
		Target_Z=Initial_Z - (Plate_Depression_Amount/4.f)*3.f;
	}
	if(*TotalMassOnPlate>=100.f){
		Target_Z=Initial_Z - Plate_Depression_Amount;
	}
	MovePlate(DeltaTime);
}

void UPressure_Plate_Move::MovePlate(float DeltaTime)
{
	//get location
	FVector CurrentLocation = GetOwner()->GetActorLocation();

	//ptr check
	if(!PressurePlateRootComponent) {return;}

	//Call Dock Plate to lock things up
	if(CurrentLocation.Z <= Final_Z + 2.f){
		PressurePlateRootComponent->DockPlate();
		return;
	}

	//change location based on Target_Z
	CurrentLocation.Z = FMath::FInterpTo(CurrentLocation.Z, Target_Z, DeltaTime, Plate_Move_Speed);

	//set location
	GetOwner()->SetActorLocation(CurrentLocation);
}

void UPressure_Plate_Move::Shutdown()
{
	bIsPlateDocked =  true;

	//stop tick
	PrimaryComponentTick.SetTickFunctionEnable(false);

	//get location
	FVector CurrentLocation = GetOwner()->GetActorLocation();

	//set final location
	CurrentLocation.Z = Final_Z;

	//set location
	GetOwner()->SetActorLocation(CurrentLocation);
}
