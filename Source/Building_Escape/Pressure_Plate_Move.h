// Copyright Vacuda 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Pressure_Plate_Move.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UPressure_Plate_Move : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPressure_Plate_Move();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	void DepressPlate(float DeltaTime);
	void RaisePlate(float DeltaTime);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	float Initial_Z;
	float Target_Z;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpens;

	UPROPERTY(EditAnywhere)
	float Plate_Depression_Speed=1.f;

	UPROPERTY(EditAnywhere)
	float Plate_Raise_Speed=1.f;

	UPROPERTY(EditAnywhere)
	float Plate_Depression_Amount=15.f;



		
};
