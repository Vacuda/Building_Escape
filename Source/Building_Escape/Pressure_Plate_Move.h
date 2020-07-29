// Copyright Vacuda 2020

#pragma once

#include "CoreMinimal.h"
#include "WeightCheck.h" //for WeightCheck
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
	void MovePlate(float DeltaTime);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	float Initial_Z;
	float Target_Z;
	float Final_Z;
	UWeightCheck* WeightCheckComponent;
	bool bIsStoneSet = false;
	void LockDoorsOpen();

	UPROPERTY(EditAnywhere)
	float Plate_Move_Speed=1.f;

	UPROPERTY(EditAnywhere)
	float Plate_Depression_Amount=50.f;

	UPROPERTY(EditAnywhere)
	float MassToMovePlate=100.f;

		
};
