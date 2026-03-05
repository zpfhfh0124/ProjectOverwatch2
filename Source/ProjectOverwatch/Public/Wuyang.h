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

	// UFUNCTION(BlueprintCallable, Category="Input")
	// void MouseLBStart();
	// UFUNCTION(BlueprintCallable, Category="Input")
	// void MouseLBTrigger(const FInputActionValue& Value);
	// UFUNCTION(BlueprintCallable, Category="Input")
	// void MouseLBComplete();
	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* SpawnPoint;
	
	AWuyang();
	
	virtual void OnB_Implementation() override;
	virtual void OnE_Implementation() override;
	virtual void OnF_Implementation() override;
	virtual void OnH_Implementation() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnMouseLB_Implementation() override;
	virtual void OnMouseRB_Implementation() override;
	virtual void OnR_Implementation() override;
	virtual void OnShift_Implementation() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	TSubclassOf<class AActor> WaterBallClass;
	
	UPROPERTY()
	AActor* CurrentWaterBall;
	
	void SpawnWaterBall();
};
