// CopyRight Saharsh
#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFrameWork/PlayerController.h"


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
	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent(){
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent){
		/*
		1. Name of key bind in project settings, exact case match
		2. IE_Pressed is defining when does the key bind come in effect. Here when it's pressed
		3. this is just a class indicating the class this input bind belongs to 
		4. 4th component specifies which function to call when key is pressed.
		 */
		InputComponent->BindAction("Grab",IE_Pressed,this, &UGrabber::Grab);
		InputComponent->BindAction("Grab",IE_Released,this,&UGrabber::Release);
	}
}

void UGrabber::FindPhysicsHandle(){
	/*
	This apprantly returns all objects of UPhysicsHandleComponent class but here 
	we  only have one object so it doesn't really matter. BE CAREFUL WHILE USING IT WITH MULTIPLE
	PHYSICS HANDLE COMPONENTS
	*/
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle==nullptr){
		UE_LOG(LogTemp, Warning, TEXT("No physics handle component found on %s"), *GetOwner()->GetName());
	}
}

FVector UGrabber::GetPlayersWorldPos() const{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation;
}
FVector UGrabber::GetPlayersReach() const{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	//draw a line from player showing the reach
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;
	return LineTraceEnd;
}

void UGrabber::Release(){
// UE_LOG(LogTemp, Warning, TEXT("Release function call successs"));
PhysicsHandle->ReleaseComponent();
}

void UGrabber::Grab(){
	FHitResult HitResult = GetFirstPhysicsBodyInReach();	
	UPrimitiveComponent* CompnentTOGrab = HitResult.GetComponent();
	if(HitResult.GetActor()){
		PhysicsHandle->GrabComponentAtLocation(CompnentTOGrab,NAME_None,GetPlayersReach());
	}

}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(PhysicsHandle->GrabbedComponent){
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}
	
}


FHitResult UGrabber::GetFirstPhysicsBodyInReach() const{ 
	
	//draw a line from player showing the reach
	DrawDebugLine(
		GetWorld(),
		GetPlayersWorldPos(),
		GetPlayersReach(), 
		FColor(0,255,0),
		false,
		0.f,
		0.f,
		5.f
	);
	//ray cast out to a certain distance
	
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
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayersWorldPos(),
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	
	return Hit;
}
