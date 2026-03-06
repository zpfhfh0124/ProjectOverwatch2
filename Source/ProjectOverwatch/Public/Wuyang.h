// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBase.h"
#include "Wuyang.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTOVERWATCH_API AWuyang : public APlayerBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* SpawnPoint;
	
	AWuyang();
	
	virtual void OnMouseLB_Implementation() override;
	virtual void OnShift_Implementation() override;
	void ShiftCoolReset();
	virtual void MouseLBComplete_Implementation() override;
	virtual void MouseLBTrigger_Implementation() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	TSubclassOf<class AActor> WaterBallClass;
	
	UPROPERTY()
	AActor* CurrentWaterBall;
	
	void SpawnWaterBall();
};
