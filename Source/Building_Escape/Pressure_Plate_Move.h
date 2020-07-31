// Copyright Vacuda 2020

#pragma once

#include "CoreMinimal.h"
// #include "PressurePlate.h" //for PressurePlate
#include "Pressure_Plate_Move.generated.h"

//forward declaration
class UPressurePlate;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UPressure_Plate_Move : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPressure_Plate_Move();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void MovePlate(float DeltaTime);
	void Shutdown();

	bool bIsPlateDocked = false;


protected:
	virtual void BeginPlay() override;

private:

	float Initial_Z;
	float Target_Z;
	float Final_Z;

	UPROPERTY()
	UPressurePlate* PressurePlateRootComponent = nullptr;

	UPROPERTY(EditAnywhere)
	float Plate_Move_Speed=1.f;

	UPROPERTY(EditAnywhere)
	float Plate_Depression_Amount=50.f;

	UPROPERTY(EditAnywhere)
	float MassToMovePlate=100.f;

		
};
