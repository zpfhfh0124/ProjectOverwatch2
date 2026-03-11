// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "OverwatchPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHUDProjectileCountUpdate, int32, CurrCount, int32, MaxCount);

/**
 * 
 */
UCLASS()
class PROJECTOVERWATCH_API AOverwatchPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	// HUD에서 받을 이벤트
	UPROPERTY(BlueprintAssignable, Category = "HUD")
	FOnHUDProjectileCountUpdate OnHUDProjectileCountUpdate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class APlayerBase> Player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurHP;
	
	__declspec(property(get=GetHP, put=SetHP)) int32 HP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ProjectileMaxCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ProjectileCurrCount;
	
	__declspec(property(get=GetProjectileCurrCount, put=SetProjectileCurrCount)) int PCC;
	
	UFUNCTION(BlueprintCallable)
	void SetPlayer(APlayerBase* PlayerCharacter);
	void SetMaxHP(int newMaxHP);
	void SetHP(int value);
	int32 GetHP();
	UFUNCTION(BlueprintCallable)
	void SetProjectileMaxCount(int newProjectileMaxCount);
	int GetProjectileMaxCount();
	UFUNCTION(BlueprintCallable)
	void SetProjectileCurrCount(int newProjectileCurrCount);
	int GetProjectileCurrCount();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	// 쿨타임
	UPROPERTY(BlueprintReadOnly)
	float CurCoolTimeShift = 0.f;
	UPROPERTY(BlueprintReadOnly)
	float CurCoolTimeE = 0.f;
	
	// HUD 쿨타임 설정
	UFUNCTION(BlueprintCallable)
	void SetCoolTimeShift(float coolTime);
	void SetCoolTimeE(float coolTime);
	
	// HUD 아이콘 설정
	UFUNCTION(BlueprintCallable)
	void SetProjectileIcon(const FString& Path);
	UFUNCTION(BlueprintCallable)
	void SetSkillEIcon(const FString& Path);
	UFUNCTION(BlueprintCallable)
	void SetSkillShiftLIcon(const FString& Path);
};
