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
	float EDuration = 3.f;
	
	virtual void BeginPlay() override;
	virtual void InitIcons() override;
	
	// 팅겨내기 지속시간 카운트
	UFUNCTION(BlueprintCallable)
	void StartDurationSkillE();
	UFUNCTION(BlueprintNativeEvent)
	void EndDurationSkillE();

	UFUNCTION(BlueprintCallable)
	void SetReflectionBoxComp(bool Enable);
	
	UFUNCTION()
	void OnHitReflection(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						  int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
