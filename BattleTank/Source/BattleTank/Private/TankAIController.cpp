// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"

//Depends on mevement component via pathfiding system

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto ControlledTank = GetPawn();
	auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();

	if (!ensure(PlayerTank && ControlledTank)) {return;}
		//Move towards the player
		MoveToActor(PlayerTank, AcceptanceRadius); //TODO check radius is in cm

		//Aim towards the player
		AimingComponent->AimAt(PlayerTank->GetActorLocation());

		AimingComponent->Fire(); //TODO limit firing rate
}