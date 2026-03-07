// Fill out your copyright notice in the Description page of Project Settings.


#include "OverwatchPlayerState.h"

#include "Net/UnrealNetwork.h"

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
	PC = newProjectileMaxCount;
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
