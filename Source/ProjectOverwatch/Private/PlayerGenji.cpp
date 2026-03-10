// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGenji.h"

#include "Components/BoxComponent.h"

APlayerGenji::APlayerGenji()
{
	ReflectionBoxComp = CreateDefaultSubobject<UBoxComponent>("ReflectionBoxComp");
	ReflectionBoxComp->SetupAttachment(GetRootComponent());
	ReflectionBoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ReflectionBoxComp->SetRelativeLocation(FVector(50, 0, 0));
	ReflectionBoxComp->SetBoxExtent(FVector(20, 50, 80));
	ReflectionBoxComp->SetCollisionProfileName(TEXT("BlueWeapon"));
}

void APlayerGenji::BeginPlay()
{
	Super::BeginPlay();

	SetReflectionBoxComp(false);

	// 쿨타임 지정
	CountShiftCoolTime = 10.0f;
	CountECoolTime = 5.0f;
	
	InitIcons();
}

void APlayerGenji::InitIcons()
{
	ProjectileIconPath = "/Script/Paper2D.PaperSprite'/Game/GT/UI/Sprites/Projectile_Genji_Sprite_0.Projectile_Genji_Sprite_0'";
	SkillEIconPath = "/Script/Paper2D.PaperSprite'/Game/GT/UI/Sprites/Skill_Genji_Sprite_1.Skill_Genji_Sprite_1'";
	SkillShiftLIconPath = "/Script/Paper2D.PaperSprite'/Game/GT/UI/Sprites/Skill_Genji_Sprite_0.Skill_Genji_Sprite_0'";
	
	Super::InitIcons();
}

void APlayerGenji::SetReflectionBoxComp(bool Enable)
{
	if (ReflectionBoxComp == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ReflectionBoxComp is Null"));
		return;
	}
	
	ReflectionBoxComp->SetActive(Enable);
}
