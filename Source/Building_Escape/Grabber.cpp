// CopyRight Saharsh

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFrameWork/PlayerController.h"
#include "Grabber.h"

#define OUT 
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	/*
	This apprantly returns all objects of UPhysicsHandleComponent class but here 
	we  only have one object so it doesn't really matter. BE CAREFUL WHILE USING IT WITH MULTIPLE
	PHYSICS HANDLE COMPONENTS

	*/
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle){

	}else{
		UE_LOG(LogTemp, Error, TEXT("No physics handle component found on %s"), *GetOwner()->GetName());
	}
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//get player viewpoint 
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	//draw a line from player showing the reach
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd, 
		FColor(0,255,0),
		false,
		0.f,
		0.f,
		5.f
	);
	//ray cast out to a certain distance
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	/*
	Future me : Read this 
	1st parameter is simply an object which records which ingame object has been hit.
	So the 4th parameter is deciding which type of objects to consider, here 
	we chose Physics body , we can choose anything. 
	The code is simply A object of FCollisionObjectQueryParams. 
	and taking ECC_physics body from the ECollisionChannel class

	And the 5th paramter is What to trace. So we trace a object with
	no particular name and ignore the player in tracing. Also the false is for
	complex collision, as we don't want complex collion here.
	*/ 
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	
	//see what it hits
	AActor* ActorHit = Hit.GetActor();
	if(ActorHit){
		UE_LOG(LogTemp,Error,TEXT("Actor has hit %s"), *(ActorHit->GetName()) ); 
	}
}

