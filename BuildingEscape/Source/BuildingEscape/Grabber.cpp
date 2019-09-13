// Owned by NinjaFrog, biatch


#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Public/DrawDebugHelpers.h"

// OUT is used to mark out_Params
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

	FindPhysicsHandleComponent();
	FindPawnInputComponent();
}

void UGrabber::FindPawnInputComponent()
{
	PawnInputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (PawnInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component FOUND"));
		PawnInputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		PawnInputComponent->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input Component not found on %s"), *GetOwner()->GetName());
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handle not found on %s"), *GetOwner()->GetName());
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent)
	{
		CalculateLineReach();
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	CalculateLineReach();

	LineTraceParams = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());

	bool bSomethingWasHit = GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		LineTraceParams
	);

	if (bSomethingWasHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("LineTrace Hit: %s"), *Hit.GetActor()->GetName())
	}

	return Hit;
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab PRESSED!"))
	auto HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* HitActor = HitResult.GetActor();

	if (HitActor)
	{
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None, // Bone is used on non-Static meshes
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // Allow rotation
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab RELEASED!"))

	PhysicsHandle->ReleaseComponent();
}

void UGrabber::CalculateLineReach()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, OUT PlayerViewPointRotation
	);

	LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
}

