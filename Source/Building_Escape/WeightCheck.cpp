// Copyright Vacuda 2020


#include "WeightCheck.h"

// Sets default values for this component's properties
UWeightCheck::UWeightCheck()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeightCheck::BeginPlay()
{
	Super::BeginPlay();


}

void UWeightCheck::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UWeightCheck::SetTotalMassOfActors(){
	//reset
	TotalMass = 0.f;

	//find all overlapping actors
	// TArray<AActor*> OverlappingActorsArr;
	// PressurePlate->GetOverlappingActors(OUT OverlappingActorsArr);

	// //add masses in the object array
	// for(AActor* Actor:OverlappingActorsArr){
	// 	TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	// }
}

