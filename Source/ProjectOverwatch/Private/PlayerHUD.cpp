// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

#include "Components/TextBlock.h"

void UPlayerHUD::SetProjectileCount(int newCount, int maxCount)
{
	TextProjectileCount->SetText(FText::Format(FText::FromString("{0}/{1}"), newCount, maxCount));
}
