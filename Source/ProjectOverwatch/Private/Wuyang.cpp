// Fill out your copyright notice in the Description page of Project Settings.


#include "Wuyang.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

void AWuyang::BeginPlay()
{
	Super::BeginPlay();
}

AWuyang::AWuyang()
{
	//sp
	SpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(FPCamera);
	SpawnPoint->ArrowSize = 1.0f;
	SpawnPoint->ArrowColor = FColor(255, 0, 0, 255);
	SpawnPoint->SetRelativeLocation(FVector(60.0f, 15.0f, -10.0f));
}

void AWuyang::OnH_Implementation()
{
	Super::OnH_Implementation();
}

void AWuyang::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWuyang::OnMouseLB_Implementation()
{
	Super::OnMouseLB_Implementation();
}

void AWuyang::OnShift_Implementation()
{
	Super::OnShift_Implementation();
}

void AWuyang::MouseLBComplete_Implementation()
{
	Super::MouseLBComplete_Implementation();
}

void AWuyang::MouseLBTrigger_Implementation()
{
	Super::MouseLBTrigger_Implementation();
}
