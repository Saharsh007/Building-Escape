// CopyRight Saharsh


#include "opendoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

// Sets default values for this component's properties
Uopendoor::Uopendoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void Uopendoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenTargetYaw += InitialYaw;
	// CloseTargetYaw = -90
	if(PressurePlate==NULL){
		UE_LOG(LogTemp,Error,TEXT("%s has opendoor but No pressure plate on PressurePlate"), *GetOwner()->GetName());
	}

}


// Called every frame
void Uopendoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// UE_LOG(LogTemp,Warning,TEXT("%f"),GetOwner()->GetActorRotation().Yaw);
	// UE_LOG(LogTemp, Warning, TEXT("this is a warning!"));
	if(PressurePlate && PressurePlate->IsOverlappingActor(TheActorThatOpensDoor)){
		OpenDoor(DeltaTime);
	}
	else {
		CloseDoor(DeltaTime);
	}

 
}
void Uopendoor::CloseDoor(float DeltaTime){
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	CurrentYaw = FMath::Lerp(CurrentYaw,CloseTargetYaw,DeltaTime*1.0f);
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation); 
}

void Uopendoor::OpenDoor(float DeltaTime){
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	CurrentYaw = FMath::Lerp(CurrentYaw,OpenTargetYaw,DeltaTime*1.0f);
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation); 
}


