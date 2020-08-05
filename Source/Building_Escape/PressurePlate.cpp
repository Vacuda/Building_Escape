// Copyright Vacuda 2020

#include "PressurePlate.h"
#include "Components/ActorComponent.h"  //general Actor Component
// #include "Engine/TriggerVolume.h" //for TriggerVolume
// #include "GameFramework/Actor.h"  //for AActor
#include "Engine/World.h"  //for getworld
#include "Kismet/GameplayStatics.h"  //for getallactorsofclass
#include "OpenDoor.h"  //for UOpenDoor
#include "Pressure_Plate_Move.h"  //for UPressure_Plate_Move

#define OUT

// Sets default values for this component's properties
UPressurePlate::UPressurePlate()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UPressurePlate::BeginPlay()
{
	Super::BeginPlay();

	//ptr check
	if(!Door_A){return;}
	if(!Door_B){return;}

	//set door components
	Door_A_Component = Door_A->FindComponentByClass<UOpenDoor>();
	Door_B_Component = Door_B->FindComponentByClass<UOpenDoor>();

	//safety check
	if(!Door_A_Component){
		UE_LOG(LogTemp, Error, TEXT("No OpenDoor component found on Door A"));
	}

	//safety check
	if(!Door_B_Component){
		UE_LOG(LogTemp, Error, TEXT("No OpenDoor component found on Door B"));
	}

}

void UPressurePlate::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MoveTriggerVolume();

	SetTotalMassOfActors();
}

void UPressurePlate::SetTotalMassOfActors(){
	//reset
	TotalMassOnPlate = 0.f;

	//ptr check
	if(!PressurePlate) {return;}

	//find all overlapping actors
	TArray<AActor*> OverlappingActorsArr;
	PressurePlate->GetOverlappingActors(OUT OverlappingActorsArr);

	//add masses in the object array
	for(AActor* Actor:OverlappingActorsArr){
		TotalMassOnPlate += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
}

void UPressurePlate::DockPlate(){

	//trigger shutdown here
	Shutdown();

	//trigger shutdown Pressure_Plate_Move
	GetOwner()->FindComponentByClass<UPressure_Plate_Move>()->Shutdown();

	//ptr check
	if(!Door_A_Component) {return;}
	if(!Door_B_Component) {return;}

	//trigger shutdown, Door A and B
	Door_A_Component->Shutdown();
	Door_B_Component->Shutdown();
}

void UPressurePlate::Shutdown(){

	bIsPlateDocked = true;

	//stop tick
	PrimaryComponentTick.SetTickFunctionEnable(false);
}

void UPressurePlate::MoveTriggerVolume(){

		// set TriggerVolume location to top of Pressure plate
		PressurePlate->SetActorLocation(GetOwner()->GetActorLocation());
		
}