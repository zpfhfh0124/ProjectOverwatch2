// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectOverwatchCharacter.h"
#include "Components/ArrowComponent.h"
#include "Player_WuYang.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTOVERWATCH_API APlayer_WuYang : public AProjectOverwatchCharacter
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* SpawnPoint;
	
	APlayer_WuYang();
	
protected:
	/** Mouse LB Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* MouseLBAction;
	
	/** Called from Input Actions for attack input */
	UFUNCTION(BlueprintCallable, Category="Input")
	void MouseLBStart();
	/** Called from Input Actions for attack input */
	UFUNCTION(BlueprintCallable, Category="Input")
	void MouseLBTrigger(const FInputActionValue& Value);
	/** Called from Input Actions for attack input */
	UFUNCTION(BlueprintCallable, Category="Input")
	void MouseLBComplete();
	
	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	TSubclassOf<class AActor> WaterBallClass;
	
	UPROPERTY()
	AActor* CurrentWaterBall;
	
	void SpawnWaterBall();
	
};
