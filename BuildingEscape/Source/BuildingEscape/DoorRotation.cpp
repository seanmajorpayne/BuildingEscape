// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorRotation.h"
#include "GameFramework/Actor.h"


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
	
	float InitialYaw = GetOwner()->GetActorRotation().Yaw;
	float CurrentYaw = InitialYaw;
	float TargetYaw = InitialYaw - 90.f;
}


// Called every frame
void UDoorRotation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	FRotator OpenDoor(0.f, 0.f, 0.f);
	OpenDoor.Yaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 1);
	GetOwner()->SetActorRotation(OpenDoor);
}

