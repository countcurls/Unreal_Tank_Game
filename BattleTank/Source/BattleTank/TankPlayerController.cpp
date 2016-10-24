// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankPlayerController.h"

#define OUT

// Sets default values
ATankPlayerController::ATankPlayerController()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();
	if (!ControlledTank) {
		UE_LOG(LogTemp, Warning, TEXT("TankPlayerController is not possessing a tank"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("TankPlayerController possessing %s"), *(ControlledTank->GetName()));
	}

}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) {
		return;
	}

	FVector HitLocation;; //Out parameter
	if (GetSightRayHitLocation(OUT HitLocation)) {
		GetControlledTank()->AimAt(HitLocation);
	}

	return;
}

//Get world location of linetrace through crosshair. Returns true if hits landscape
bool ATankPlayerController::GetSightRayHitLocation(OUT FVector& HitLocation) const
{
	HitLocation = FVector(0.f, 0.f, 0.f);

	///Find crosshaird position
	int32 ViewportSizeX, ViewportSixeY;
	GetViewportSize(OUT ViewportSizeX, OUT ViewportSixeY);

	FVector2D ScreenLocation = FVector2D(ViewportSizeX * CrossHairXLocation, ViewportSixeY * CrossHairYLocation);
	FVector LookDirection;

	///Deproject the screen position of the crosshair to a world direction
	if (GetLookDirection(ScreenLocation, OUT LookDirection)) {
		///Line trace along that look direction, and see what we hit (up to a max range)
		GetLookVectorHitLocation(LookDirection, OUT HitLocation, LineTraceRange);
	}
	return true;
}

//Deprojects screen position and assigns result to OUT LookDirection. Returns true if successful
bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, OUT FVector& LookDirection) const
{
	FVector CameraWorldLocation; ///Temp variable needed to call deproject function

	return DeprojectScreenPositionToWorld(
		ScreenLocation.X
		, ScreenLocation.Y
		, OUT CameraWorldLocation
		, OUT LookDirection
	);
}

//Line trace in the look direction from camera position, return true if successful, assign OUT HitLocation
bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, OUT FVector& HitLocation, float Range) const
{
	FHitResult HitResult;
	FVector Start = PlayerCameraManager->GetCameraLocation()
		, End = Start + (LookDirection * Range);

	if (GetWorld()->LineTraceSingleByChannel(
			OUT HitResult
			, OUT Start
			, OUT End
			, ECollisionChannel::ECC_Visibility) ///Lint trace against all visible entities
		)
	{
		HitLocation = HitResult.Location; 
		return true;
	}
	else {
		HitLocation = FVector(0.f); ///Hit unsuccessful, set hit location to 0, 0, 0
		return false;
	}


}

