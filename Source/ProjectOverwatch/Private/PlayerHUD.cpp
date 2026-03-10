// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

#include "OverwatchPlayerState.h"
#include "PaperSprite.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UPlayerHUD::SetProjectileIcon(UPaperSprite* Icon)
{
	ProjectileIcon->SetBrushFromAtlasInterface(Icon);
}

void UPlayerHUD::SetProjectileCount(int newCount, int maxCount)
{
	TextProjectileCount->SetText(FText::Format(FText::FromString("{0}/{1}"), newCount, maxCount));
}

void UPlayerHUD::SetSkillEIcon(UPaperSprite* Icon)
{
	SkillEIcon->SetBrushFromAtlasInterface(Icon);
}

void UPlayerHUD::SetSkillShiftLIcon(UPaperSprite* Icon)
{
	SkillShiftLIcon->SetBrushFromAtlasInterface(Icon);
}

void UPlayerHUD::SetCoolTimeSkillShiftL(const int coolTime)
{
	if (coolTime <= 0)
	{
		CoolTimeShiftL->SetText(FText::FromString("0"));
		CoolTimeShiftL->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		CoolTimeShiftL->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPlayerHUD::SetCoolTimeSkillE(const int coolTime)
{
	if (coolTime <= 0)
	{
		CoolTimeE->SetText(FText::FromString("0"));
		CoolTimeE->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		CoolTimeE->SetVisibility(ESlateVisibility::Visible);
	}
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
