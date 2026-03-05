// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JunRocket.h"
#include "PlayerBase.h"
#include "Soldier76.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTOVERWATCH_API ASoldier76 : public APlayerBase
{
	GENERATED_BODY()
	


public:
	ASoldier76();

protected:
	// --- Soldier weapon/aim ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<AJunRocket> RocketFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	USceneComponent* FirePoint;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TEnumAsByte<ECollisionChannel> AimTraceChannel = ECC_Visibility;

	// Sprint
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float WalkSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float SprintSpeed = 1200.f;

	// --- Overrides (PlayerBase input entry points) ---
	virtual void MoveInput(const FInputActionValue& Value) override;

	virtual void OnShift_Implementation() override;
	virtual void StopShift_Implementation() override;

	virtual void OnMouseRB_Implementation() override; // 로켓 발사
	// 필요하면 LB(일반 총)도 여기서 구현
	// virtual void OnMouseLB_Implementation() override;

private:
	bool GetAimPointFromCamera(FVector& OutAimPoint, FVector& OutTraceEnd) const;
	void FireRocket();

};