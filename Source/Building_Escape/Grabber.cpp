// Copyright Vacuda 2020

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "GameFramework/PlayerController.h"

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

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("grabber pressed"));

	//get and set players viewpoint
	FVector PlayerViewPoint_Location;
	FRotator PlayerViewPoint_Rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPoint_Location, 
		OUT PlayerViewPoint_Rotation
	);

	//end of line trace
	FVector LineTraceEnd = PlayerViewPoint_Location + PlayerViewPoint_Rotation.Vector()*Reach;





	FHitResult HitResult = GetFirstPhysicsBodyWithinReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	//if something hits, attach physics component
	if(HitResult.GetActor()){
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			LineTraceEnd
		);
	}

}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
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

	//get and set players viewpoint
	FVector PlayerViewPoint_Location;
	FRotator PlayerViewPoint_Rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPoint_Location, 
		OUT PlayerViewPoint_Rotation
	);

	//end of line trace
	FVector LineTraceEnd = PlayerViewPoint_Location + PlayerViewPoint_Rotation.Vector()*Reach;



	if(PhysicsHandle->GrabbedComponent){
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}

}

FHitResult const UGrabber::GetFirstPhysicsBodyWithinReach()
{
	//get and set players viewpoint
	FVector PlayerViewPoint_Location;
	FRotator PlayerViewPoint_Rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPoint_Location, 
		OUT PlayerViewPoint_Rotation
	);

	//end of line trace
	FVector LineTraceEnd = PlayerViewPoint_Location + PlayerViewPoint_Rotation.Vector()*Reach;

	//list of collisions to ignore...I think.
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	//set Hit, if object type if physics body
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPoint_Location,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	//get object that hit, print name
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit){
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()));
	}

	return Hit;
}