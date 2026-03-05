// Fill out your copyright notice in the Description page of Project Settings.


// Soldier76.cpp
#include "Soldier76.h"

#include "JunRocket.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"

ASoldier76::ASoldier76()
{
	// FirePoint 만들기 (AJunCharacter에서 하던 것 그대로)
	FirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("FirePoint"));
	FirePoint->SetupAttachment(RootComponent);

	// MoveComp는 PlayerBase에서 이미 세팅하지만, 안전하게 초기 속도 세팅
	if (UCharacterMovementComponent* CMC = GetCharacterMovement())
	{
		CMC->MaxWalkSpeed = WalkSpeed;
	}
}

void ASoldier76::OnShift_Implementation()
{
	bIsShifting = true;
}

void ASoldier76::StopShift_Implementation()
{
	bIsShifting = false;

	if (UCharacterMovementComponent* CMC = GetCharacterMovement())
	{
		CMC->MaxWalkSpeed = WalkSpeed;
	}
}

void ASoldier76::MoveInput(const FInputActionValue& Value)
{
	// PlayerBase::MoveInput의 “이동 계산”은 그대로 쓰되,
	// Soldier 스프린트 조건(Shift + 전진일 때만 SprintSpeed)을 여기서 적용
	const FVector2D Axis = Value.Get<FVector2D>();
	if (!Controller) return;

	// ✅ 전후가 Axis.X 라는 너의 규칙 유지
	const float Forward = Axis.X;
	const bool bSprintNow = bIsShifting && (Forward > 0.f);

	if (UCharacterMovementComponent* CMC = GetCharacterMovement())
	{
		CMC->MaxWalkSpeed = bSprintNow ? SprintSpeed : WalkSpeed;
	}

	// 아래는 PlayerBase의 MoveInput과 동일
	const FRotator ControlRot = Controller->GetControlRotation();
	const FRotator YawRot(0.f, ControlRot.Yaw, 0.f);

	const FVector ForwardDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
	const FVector RightDir   = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDir, Axis.X);
	AddMovementInput(RightDir,   Axis.Y);
}

void ASoldier76::OnMouseRB_Implementation()
{
	FireRocket();
}

void ASoldier76::FireRocket()
{
	if (!FirePoint || !RocketFactory) return;
	if (!FPCamera) return; // PlayerBase의 카메라 사용

	FVector AimPoint, TraceEnd;
	if (!GetAimPointFromCamera(AimPoint, TraceEnd)) return;

	const FVector MuzzleLoc = FirePoint->GetComponentLocation();
	const FRotator SpawnRot = (AimPoint - MuzzleLoc).Rotation();

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = this;

	GetWorld()->SpawnActor<AJunRocket>(RocketFactory, MuzzleLoc, SpawnRot, Params);
}

bool ASoldier76::GetAimPointFromCamera(FVector& OutAimPoint, FVector& OutTraceEnd) const
{
	if (!FPCamera) return false;

	const FVector Start = FPCamera->GetComponentLocation();
	const FVector End   = Start + (FPCamera->GetForwardVector() * 30000.f);
	OutTraceEnd = End;

	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(SoldierCameraAimTrace), true);
	Params.AddIgnoredActor(this);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		AimTraceChannel,
		Params
	);

	OutAimPoint = bHit ? Hit.ImpactPoint : End;

	// (선택) 디버그
	// DrawDebugLine(GetWorld(), Start, bHit ? Hit.ImpactPoint : End, FColor::Green, false, 1.f, 0, 1.f);

	return true;
}