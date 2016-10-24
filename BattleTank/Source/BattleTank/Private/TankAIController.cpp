// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankAIController.h"




//// Sets default values
//ATankAIController::ATankAIController()
//{
//	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;
//
//}

// Called when the game starts or when spawned
void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();
	if (!ControlledTank) {
		UE_LOG(LogTemp, Warning, TEXT("TankAIController is not possessing a tank"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("TankAIController possessing %s"), *(ControlledTank->GetName()));
	}

	auto PlayerTank = GetPlayerTank();
	if (!PlayerTank) {
		UE_LOG(LogTemp, Warning, TEXT("TankAIController %s, can't find Player Tank"), *(ControlledTank->GetName()));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Player tank found, possessing %s"), *(PlayerTank->GetName()));
	}
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetPlayerTank()) {
		// TODO move towards player

		//Aim towards player
		AimAtPlayer();

		//Fire if ready

	}
}

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const
{
	ATank* PlayerPawn = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (!PlayerPawn) {
		return nullptr;
	}
	else {
		return PlayerPawn;
	}
}

void ATankAIController::AimAtPlayer() const {
	GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());
}
