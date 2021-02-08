// CopyRight Saharsh

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/TriggerVolume.h"
#include "opendoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API Uopendoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	Uopendoor();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();
	void FindPressurePlate();

	bool OpenDoorSound = true;
	bool CloseDoorSound = false;

private:
	float InitialYaw;
	float CurrentYaw; 
	UPROPERTY(EditAnywhere)
	float OpenAngle = -180.0f;
	// UPROPERTY(EditAnywhere)
	float CloseAngle = -90.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* TheActorThatOpensDoor = nullptr;

	float DoorLastOpened=0.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay=0.5f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 1.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed=3.0f;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoor=50.f; 

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;

};
