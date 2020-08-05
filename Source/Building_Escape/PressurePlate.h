// Copyright Vacuda 2020

#pragma once

#include "CoreMinimal.h"
// #include "Components/ActorComponent.h"  //general Actor Component
#include "Engine/TriggerVolume.h" //for TriggerVolume
#include "GameFramework/Actor.h"  //for AActor
// #include "Engine/World.h"  //for getworld
// #include "Pressure_Plate_Move.h"  //for UPressure_Plate_Move
// #include "OpenDoor.h"  //for UOpenDoor
// #include "Kismet/GameplayStatics.h"  //for getallactorsofclass
#include "PressurePlate.generated.h"

//forward declaration
class UOpenDoor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UPressurePlate : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPressurePlate();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float TotalMassOnPlate = 0.f;
	bool bIsPlateDocked = false;
	void DockPlate();
	void Shutdown();
	void MoveTriggerVolume();


protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	AActor* Door_A = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* Door_B = nullptr;

	UPROPERTY()
	UOpenDoor* Door_A_Component = nullptr;

	UPROPERTY()
	UOpenDoor* Door_B_Component = nullptr;

	void SetTotalMassOfActors();

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

		
};
