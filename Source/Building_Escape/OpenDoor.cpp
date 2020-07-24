// Copyright Vacuda 2020


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Math/Rotator.h"

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


	TargetYaw = GetOwner()->GetActorRotation().Yaw + 90.0f;

	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw);

	FRotator CurrentRotation = GetOwner()->GetActorRotation();

	CurrentRotation.Yaw = FMath::Lerp(CurrentRotation.Yaw, TargetYaw, 0.03f);

	GetOwner()->SetActorRotation(CurrentRotation);


}

