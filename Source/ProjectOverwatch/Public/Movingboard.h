#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingBoard.generated.h"

UCLASS()
class PROJECTOVERWATCH_API AMovingBoard : public AActor
{
	GENERATED_BODY()

public:
	AMovingBoard();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComp;

	FVector StartLocation;
	FVector TargetLocation;
	bool bMovingToTarget;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveDistance = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bMoveUpVector = false;
};