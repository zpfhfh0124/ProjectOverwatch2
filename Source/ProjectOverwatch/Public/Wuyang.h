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
	virtual void OnB_Implementation() override;
	virtual void OnE_Implementation() override;
	virtual void OnF_Implementation() override;
	virtual void OnH_Implementation() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnMouseLB_Implementation() override;
	virtual void OnMouseRB_Implementation() override;
	virtual void OnR_Implementation() override;
	virtual void OnShift_Implementation() override;
};
