// Copyright Vacuda 2020

#define OUT

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	//set physics handle connection
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	//set Input connection
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	//set Input Bindings
	if(InputComponent){
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}

}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("grabber pressed"));
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("grabber released"));
}


// Called every frame
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

	//draw debug line!
	DrawDebugLine(
		GetWorld(),
		PlayerViewPoint_Location,
		LineTraceEnd,
		FColor(0,255, 0),
		false,
		0.f,
		0,
		5.f
	);

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







}

