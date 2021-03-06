// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorRotation.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"

#define OUT


// Sets default values for this component's properties
UDoorRotation::UDoorRotation()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDoorRotation::BeginPlay()
{
	Super::BeginPlay();
	
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw = InitialYaw - 90.f;

	if(!PressurePlate)
	{
		FString name = GetOwner()->GetName();
		UE_LOG(LogTemp, Warning, TEXT("%s has an open door component but no pressure plate set."), *name);
	}

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	FindAudioComponent();
}

void UDoorRotation::FindAudioComponent()
{
	DoorSound = GetOwner()->FindComponentByClass<UAudioComponent>();
	if(!DoorSound)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing DoorSound component on %s"), *GetOwner()->GetName());
	}
}


// Called every frame
void UDoorRotation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(PressurePlate && TotalMassOfActors() >= MassToOpenDoors)
	{
		OpenDoor(DeltaTime);
	}
	else
	{
		CloseDoor(DeltaTime);
	}
}

// Open door upon event
void UDoorRotation::OpenDoor(float DeltaTime)
{
	CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	FRotator Increment(0.f, 0.f, 0.f);
	Increment.Yaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 1);
	GetOwner()->SetActorRotation(Increment);

	CloseDoorSound = false;
	if(!DoorSound) {return;}
	if(!OpenDoorSound && !DoorSound->IsPlaying())
	{
		DoorSound->Play();
		OpenDoorSound = true;
	}
}

// Close door upon event
void UDoorRotation::CloseDoor(float DeltaTime)
{
	CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	FRotator Increment(0.f, 0.f, 0.f);
	Increment.Yaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, 2);
	GetOwner()->SetActorRotation(Increment);

	OpenDoorSound = false;
	if(!DoorSound) {return;}
	if(!CloseDoorSound && !DoorSound->IsPlaying())
	{
		DoorSound->Play();
		CloseDoorSound = true;
	}
}

float UDoorRotation::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	// Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	
	// Add up their masses

	return TotalMass;
}