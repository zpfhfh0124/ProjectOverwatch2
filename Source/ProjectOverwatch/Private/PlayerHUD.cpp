// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

#include "OverwatchPlayerState.h"
#include "Components/TextBlock.h"

void UPlayerHUD::SetProjectileIcon(UImage* Icon)
{
	ProjectileIcon = Icon;
}

void UPlayerHUD::SetProjectileCount(int newCount, int maxCount)
{
	TextProjectileCount->SetText(FText::Format(FText::FromString("{0}/{1}"), newCount, maxCount));
}

void UPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PC = GetOwningPlayer();
	AOverwatchPlayerState* PS = PC->GetPlayerState<AOverwatchPlayerState>();

	if (PS)
	{
		PS->OnHUDProjectileCountUpdate.AddDynamic(this, &UPlayerHUD::SetProjectileCount);
	}
}
