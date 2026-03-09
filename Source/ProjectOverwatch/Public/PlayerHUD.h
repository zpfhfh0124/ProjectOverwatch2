// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTOVERWATCH_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 소모형 무기(투사형) 아이콘
	UPROPERTY(meta=(BindWidget))
	class UImage* ProjectileIcon;
	
	// 소모형 무기(투사형) 잔량 
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TextProjectileCount;
	
	// Skill 좌Shift 버튼
	UPROPERTY(meta=(BindWidget))
	class UButton* BtnShiftL;
	
	// Skill E 버튼
	UPROPERTY(meta=(BindWidget))
	class UButton* BtnE;
	
	// 소모형 무기 아이콘 설정
	void SetProjectileIcon(UImage* Icon);
	
	// 소모형 무기 잔량 표시
	UFUNCTION()
	void SetProjectileCount(int newCount, int maxCount);

	virtual void NativeConstruct() override;
};
