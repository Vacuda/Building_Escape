// Copyright Vacuda 2020

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h" //for TriggerVolume
#include "GameFramework/Actor.h"  //for AActor
#include "WeightCheck.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UWeightCheck : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeightCheck();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float TotalMassOfActors();

private:
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;



		
};
