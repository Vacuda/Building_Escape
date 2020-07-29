// Copyright Vacuda 2020

#pragma once

#include "CoreMinimal.h"
#include "WeightCheck.h"  //for WeightCheck
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	UOpenDoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void MoveDoor(float DeltaTime);

private:
	float Initial_Yaw;
	float Target_Yaw;
	float Final_Yaw;
	UWeightCheck* WeightCheckComponent;
	bool bIsDoorSet = false;
	void RotationCorrection(float* Yaw);

	UPROPERTY(EditAnywhere)
	float DoorMoveSpeed=2.f;

	UPROPERTY(EditAnywhere)
	float DoorAngleChange = 90.f;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoor = 100.f;
};
