// Copyright Vacuda 2020

#pragma once

#include "CoreMinimal.h"
// #include "Kismet/GameplayStatics.h"  //for getallactorsofclass
// #include "Engine/World.h"  //for getworld
// #include "PressurePlate.h"  //for PressurePlate
#include "OpenDoor.generated.h"

//forward declaration
class UPressurePlate;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	UOpenDoor();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void MoveDoor(float DeltaTime);
	void Shutdown();

	bool bIsDoorDocked = false;

protected:
	virtual void BeginPlay() override;

private:
	float Initial_Yaw;
	float Target_Yaw;
	float Final_Yaw;

	void RotationCorrection(float* Yaw);

	UPROPERTY()
	UPressurePlate* PressurePlateComponent = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* DoorTriggerActor = nullptr;

	UPROPERTY(EditAnywhere)
	float DoorMoveSpeed=2.f;

	UPROPERTY(EditAnywhere)
	float DoorAngleChange = 90.f;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoor = 100.f;
};
