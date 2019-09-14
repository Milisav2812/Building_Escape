// Owned by NinjaFrog, biatch

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Calculates the LineTraceEnd vector based on current Player ViewPoint Position
	void CalculateLineReach();

	// Returns an Actor that is at the end of our Reach if it is a PhysicsBody Object Type
	FHitResult GetFirstPhysicsBodyInReach();
	
	// Called when the game starts
	virtual void BeginPlay() override;

	void FindPawnInputComponent();
	void FindPhysicsHandleComponent();

private:
	float Reach = 100;

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	FVector LineTraceEnd;
	FHitResult Hit;
	FCollisionQueryParams LineTraceParams;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* PawnInputComponent = nullptr;

	void Grab();
	void Release();
};
