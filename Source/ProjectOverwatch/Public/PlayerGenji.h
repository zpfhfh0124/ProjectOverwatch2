// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBase.h"
#include "PlayerGenji.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTOVERWATCH_API APlayerGenji : public APlayerBase
{
	GENERATED_BODY()

public :
	APlayerGenji();
	
	// 팅겨내기 콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UBoxComponent> ReflectionBoxComp;

	// 팅겨내기 지속시간
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float EDuration = 2.f;
	
	virtual void BeginPlay() override;
	virtual void InitIcons() override;

	UFUNCTION(BlueprintCallable)
	void SetReflectionBoxComp(bool Enable);
};
