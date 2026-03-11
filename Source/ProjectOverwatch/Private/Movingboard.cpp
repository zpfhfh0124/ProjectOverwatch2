#include "MovingBoard.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AMovingBoard::AMovingBoard()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = StaticMeshComp;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("/Script/Engine.StaticMesh'/Game/Jun/Art/movingboardmodelfbx.movingboardmodelfbx'")
	);
	if (MeshAsset.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(MeshAsset.Object);
	}
}

void AMovingBoard::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();

	FVector MoveDirection = bMoveUpVector ? GetActorUpVector() : GetActorForwardVector();
	TargetLocation = StartLocation + MoveDirection * MoveDistance;

	bMovingToTarget = true;
}

void AMovingBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();
	FVector Destination = bMovingToTarget ? TargetLocation : StartLocation;

	FVector NewLocation = FMath::VInterpConstantTo(
		CurrentLocation,
		Destination,
		DeltaTime,
		MoveSpeed
	);
	SetActorLocation(NewLocation);

	if (FVector::Dist(NewLocation, Destination) < 10.f)
	{
		bMovingToTarget = !bMovingToTarget;
	}
}