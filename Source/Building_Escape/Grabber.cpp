// Copyright Vacuda 2020

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandle();
	SetInputComponentAndBindings();
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
}

void UGrabber::SetInputComponentAndBindings()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if(InputComponent){
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if holding an item
	if(PhysicsHandle->GrabbedComponent){

		//store OldYaw
		float OldYaw = VP_Structure.PlayerViewPoint_Rotation.Yaw;

		BuildPlayerViewPointStructure();

		//set location of target object
		PhysicsHandle->SetTargetLocation(VP_Structure.LineTraceEnd);


		//Get YawChange, Set new object rotation
		float YawChange = VP_Structure.PlayerViewPoint_Rotation.Yaw - OldYaw;
		VP_Structure.ObjectRotation.Yaw += YawChange;
		PhysicsHandle->SetTargetRotation(VP_Structure.ObjectRotation);
	}
}

void UGrabber::Grab()
{
	BuildPlayerViewPointStructure();
	
	//if something hits, attach physics component
	if(VP_Structure.HitResult.GetActor()){
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			VP_Structure.ComponentToGrab,
			NAME_None,
			VP_Structure.LineTraceEnd,
			VP_Structure.ObjectRotation
		);
	}

}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::BuildPlayerViewPointStructure()
{
	//set Location and Rotation of Player
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT VP_Structure.PlayerViewPoint_Location, 
		OUT VP_Structure.PlayerViewPoint_Rotation
	);

	//Set End of Line Trace On First Physics Object, within REACH
	VP_Structure.LineTraceEnd = VP_Structure.PlayerViewPoint_Location + VP_Structure.PlayerViewPoint_Rotation.Vector()*Reach;

	//list of collisions to ignore, just your pawn
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	//set HitResult, if object type if physics body
	GetWorld()->LineTraceSingleByObjectType(
		OUT VP_Structure.HitResult,
		VP_Structure.PlayerViewPoint_Location,
		VP_Structure.LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	//set ComponentToGrab;
	VP_Structure.ComponentToGrab = VP_Structure.HitResult.GetComponent();

	//get target object rotation
	if(VP_Structure.HitResult.GetActor()){
		VP_Structure.ObjectRotation = VP_Structure.HitResult.GetActor()->GetActorRotation();
	}
}