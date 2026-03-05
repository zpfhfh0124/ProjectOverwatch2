// Fill out your copyright notice in the Description page of Project Settings.


#include "Wuyang.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

void AWuyang::BeginPlay()
{
	Super::BeginPlay();
}

// void AWuyang::MouseLBStart()
// {
// }
//
// void AWuyang::MouseLBTrigger(const FInputActionValue& Value)
// {
// }
//
// void AWuyang::MouseLBComplete()
// {
// }

void AWuyang::SetupPlayerInputComponent(UInputComponent* InputCompo)
{
	Super::SetupPlayerInputComponent(InputCompo);
	
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

void AWuyang::OnB_Implementation()
{
	Super::OnB_Implementation();
}

void AWuyang::OnE_Implementation()
{
	Super::OnE_Implementation();
}

void AWuyang::OnF_Implementation()
{
	Super::OnF_Implementation();
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

void AWuyang::OnMouseRB_Implementation()
{
	Super::OnMouseRB_Implementation();
}

void AWuyang::OnR_Implementation()
{
	Super::OnR_Implementation();
}

void AWuyang::OnShift_Implementation()
{
	Super::OnShift_Implementation();
}
