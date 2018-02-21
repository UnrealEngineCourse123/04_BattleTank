// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "BattleTank.h"
#include "TankAimingComponent.h"
#include "Tank.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }

	FoundAimingComponent(AimingComponent);

	auto ControlledTank = GetPawn();
	if (!ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller not possesing a tank"))
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller possesing %s"), *(ControlledTank->GetName()))
	}
}

void ATankPlayerController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PossesedTank = Cast<ATank>(InPawn);
		if (!ensure(PossesedTank)) { return; }

		//Subscribe our local method to the tank's death event
		PossesedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPossesedTankDeath);
	}
}

void ATankPlayerController::OnPossesedTankDeath()
{
	StartSpectatingOnly();
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetPawn()) { return; } //e.g. if not possesing
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }
	FVector HitLocation; //Out parameter
	bool bGotHitLocation = GetSightRayHitLocation(HitLocation);
	if (GetSightRayHitLocation(HitLocation)) //Has "side effect", is going to line trace
	{
		AimingComponent->AimAt(HitLocation);
		
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	//Find the crosshair position in pixel coordinates
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	auto ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);

	//"De-project" the screen position of the crosshair to a world direction
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		//Line-trace along that LookDirection, and see what we hit (up to max range)
		return GetLookVectorHitLocation(LookDirection, HitLocation);
	}
	
	return false;
}
bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);
	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Camera)
		){
	/*	DrawDebugLine(
			GetWorld(),
			StartLocation,
			EndLocation,
			FColor(255, 0, 0),
			false, -1, 0,
			12.333
		);*/
	
		HitLocation = HitResult.Location;
		return true;
	}
		HitLocation = FVector(0);
		return false;
}
bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector CameraWorldLocation; //To be discarded
	return DeprojectScreenPositionToWorld(
		ScreenLocation.X,
		ScreenLocation.Y, 
		CameraWorldLocation, 
		LookDirection);
}


