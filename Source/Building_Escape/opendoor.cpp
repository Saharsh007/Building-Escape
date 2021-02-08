// CopyRight Saharsh
#include "opendoor.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

#define OUT

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
	// OpenAngle += InitialYaw;
	// CloseAngle = -90
	if(PressurePlate==NULL){
		UE_LOG(LogTemp,Error,TEXT("%s has opendoor but No pressure plate on PressurePlate"), *GetOwner()->GetName());
	}
	FindAudioComponent();
	FindPressurePlate();
}

void  Uopendoor::FindPressurePlate(){
	if(!AudioComponent){
		UE_LOG(LogTemp,Error,TEXT("%s has no pressure plate set on it"), *GetOwner()->GetName());
	}
}

void Uopendoor::FindAudioComponent() {
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if(!AudioComponent){
		UE_LOG(LogTemp,Error,TEXT("%s Missing Audio Component"), *GetOwner()->GetName());
	}
}


// Called every frame
void Uopendoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	float CurrTime = GetWorld()->GetTimeSeconds();
	// UE_LOG(LogTemp,Warning,TEXT("%f"),GetOwner()->GetActorRotation().Yaw);
	// UE_LOG(LogTemp, Warning, TEXT("this is a warning!"));
	if( TotalMassOfActors() >= MassToOpenDoor){
		OpenDoor(DeltaTime);
		DoorLastOpened = CurrTime;
	}
	else if( CurrTime- DoorLastOpened >= DoorCloseDelay){
		CloseDoor(DeltaTime);
	}

 
}
void Uopendoor::CloseDoor(float DeltaTime){
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	CurrentYaw = FMath::Lerp(CurrentYaw,CloseAngle,DeltaTime*DoorCloseSpeed );
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation); 
	//if door is closed now the play the sound
	if(CloseDoorSound){
		AudioComponent->Play();
		CloseDoorSound = false;
		OpenDoorSound = true;
	}
	
}

void Uopendoor::OpenDoor(float DeltaTime){
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	CurrentYaw = FMath::Lerp(CurrentYaw,OpenAngle,DeltaTime*DoorOpenSpeed );
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation); 
	if(OpenDoorSound){
		AudioComponent->Play();
		CloseDoorSound = true;
		OpenDoorSound = false;
	}
}

float Uopendoor::TotalMassOfActors() const{
	float TotalMass = 0.f;
	TArray<AActor*> OverLappingActors;
	if(!PressurePlate){return TotalMass;}

	PressurePlate->GetOverlappingActors(OverLappingActors,nullptr);
	for(AActor* TempActors : OverLappingActors){
		TotalMass += TempActors->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		// UE_LOG(LogTemp,Warning,TEXT("actors are %s"),*TempActors->GetName());

	}
	return TotalMass;
}


