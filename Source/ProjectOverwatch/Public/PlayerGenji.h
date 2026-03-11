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
	
	// 쿨타임
	float CurrCoolTimeShift = 0.f;
	float CurrCoolTimeE = 0.f;
	
	// 팅겨내기 콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UBoxComponent> ReflectionBoxComp;

	// 팅겨내기 지속시간
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float EDuration = 3.f;
	
	// 질풍참 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HayatePower = 5000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HayateDuration = 2.f;
	
	// 질풍참 애니메이션 몽타쥬
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	class UAnimMontage* HayateAnimMontage;

	FTimerHandle HayateTimerHandle;
	
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;
	virtual void InitIcons() override;
	virtual void OnShift_Implementation() override;
	
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
	
	// 질풍참
	UFUNCTION(BlueprintNativeEvent)
	void AttackHayate();
	UFUNCTION(BlueprintNativeEvent)
	void EndHayate();
	
	virtual void OnRep_PlayerState() override;
	virtual void PossessedBy(AController* NewController) override;
};
