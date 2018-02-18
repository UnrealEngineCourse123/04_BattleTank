// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "Engine/World.h"
#include "TankAIController.generated.h"

/**
 * 
 */

UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
	private:

		virtual void BeginPlay() override;

		virtual void Tick(float DeltaTime) override;

	protected:
		//how close can the AI tank get
		UPROPERTY(EditDefaultsOnly, Category="Setup")
		float AcceptanceRadius = 8000;
};
