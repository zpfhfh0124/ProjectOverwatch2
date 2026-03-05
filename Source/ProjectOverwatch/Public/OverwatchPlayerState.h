// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "OverwatchPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHUDProjectileCountUpdate);

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
	
	__declspec(property(get=GetProjectileCurrCount, put=SetProjectileCurrCount)) int PC;
	
	UFUNCTION(BlueprintCallable)
	void SetPlayer(APlayerBase* PlayerCharacter);
	void SetMaxHP(int newMaxHP);
	void SetHP(int value);
	int32 GetHP();
	void SetProjectileMaxCount(int newProjectileMaxCount);
	int GetProjectileMaxCount();
	void SetProjectileCurrCount(int newProjectileCurrCount);
	int GetProjectileCurrCount();
	
	// 이벤트 함수
	UFUNCTION()
	void OnRep_ProjectileCount();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
