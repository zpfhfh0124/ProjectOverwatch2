// Fill out your copyright notice in the Description page of Project Settings.


#include "OverwatchPlayerState.h"

#include "PlayerHUD.h"
#include "ProjectOverwatchPlayerController.h"
#include "Components/Image.h"
#include "Net/UnrealNetwork.h"
#include "PaperSprite.h"

void AOverwatchPlayerState::SetPlayer(APlayerBase* PlayerCharacter)
{
	Player = PlayerCharacter;
}

void AOverwatchPlayerState::SetMaxHP(int newMaxHP)
{
	MaxHP = newMaxHP;
	HP = MaxHP;
}

void AOverwatchPlayerState::SetHP(int value)
{
	CurHP = value;
	CurHP = FMath::Clamp(CurHP, 0, MaxHP);
}

int32 AOverwatchPlayerState::GetHP()
{
	return CurHP;	
}

void AOverwatchPlayerState::SetProjectileMaxCount(int newProjectileMaxCount)
{
	ProjectileMaxCount = newProjectileMaxCount;
	PCC = newProjectileMaxCount;
}

int AOverwatchPlayerState::GetProjectileMaxCount()
{
	return ProjectileMaxCount;
}

void AOverwatchPlayerState::SetProjectileCurrCount(int newProjectileCurrCount)
{
	ProjectileCurrCount = newProjectileCurrCount;
	OnHUDProjectileCountUpdate.Broadcast(ProjectileCurrCount, ProjectileMaxCount);
}

int AOverwatchPlayerState::GetProjectileCurrCount()
{
	return ProjectileCurrCount;
}

void AOverwatchPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AOverwatchPlayerState, ProjectileCurrCount);
}

void AOverwatchPlayerState::SetCoolTimeShift(float coolTime)
{
}

void AOverwatchPlayerState::SetCoolTimeE(float coolTime)
{
}

void AOverwatchPlayerState::SetProjectileIcon(const FString& Path)
{
	auto* PC = Cast<AProjectOverwatchPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		auto* Icon = LoadObject<UPaperSprite>(nullptr, *Path);
		PC->GetPlayerHUD()->SetProjectileIcon(Icon);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SetProjectileIcon Icon을 못찾았다!"));
	}
}

void AOverwatchPlayerState::SetSkillEIcon(const FString& Path)
{
	auto* PC = Cast<AProjectOverwatchPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		auto* Icon = LoadObject<UPaperSprite>(nullptr, *Path);
		PC->GetPlayerHUD()->SetSkillEIcon(Icon);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SetSkillEIcon Icon을 못찾았다!"));
	}
}

void AOverwatchPlayerState::SetSkillShiftLIcon(const FString& Path)
{
	auto* PC = Cast<AProjectOverwatchPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		auto* Icon = LoadObject<UPaperSprite>(nullptr, *Path);
		PC->GetPlayerHUD()->SetSkillShiftLIcon(Icon);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SetSkillShiftLIcon Icon을 못찾았다!"));
	}
}
