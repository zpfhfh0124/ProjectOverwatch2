// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#define THIRD_PERSON_ARM_LENGTH 300.0f

#include "PlayerBase.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
APlayerBase::APlayerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 0.0f;
	SpringArm->bUsePawnControlRotation = true;
	
	FPCamera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	FPCamera->SetupAttachment(SpringArm);
	
	MoveComp = GetCharacterMovement();
	MoveComp->bCanWalkOffLedges = true;
	MoveComp->bCanWalkOffLedgesWhenCrouching = true;
	// MoveComp->JumpZVelocity = 600.f;
	MoveComp->NavAgentProps.bCanCrouch = true;
	MoveComp->AirControl = 0.5f;
	
	InitializeInputs();
}

void APlayerBase::TogglePerspective_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("TogglePerspective_Implementation"));
	if (PerspectiveMode == EPerspectiveMode::FirstPerson) SetPerspectiveMode(EPerspectiveMode::ThirdPerson);
	else SetPerspectiveMode(EPerspectiveMode::FirstPerson);
}


void APlayerBase::InitializeInputs()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_Asset(TEXT("/Game/GT/Inputs/IMC_GT.IMC_GT"));
	if (IMC_Asset.Succeeded())
	{
		InputMappingContext = IMC_Asset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Move_Asset(TEXT("/Game/GT/Inputs/IA_GT_Move.IA_GT_Move"));
	if (IA_Move_Asset.Succeeded())
	{
		IA_Move = IA_Move_Asset.Object;
	}
	// 3. IA 파일 연결 (예: 시점 회전)
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_LookYaw_Asset(TEXT("/Game/GT/Inputs/IA_GT_LookYaw.IA_GT_LookYaw"));
	if (IA_LookYaw_Asset.Succeeded())
	{
		IA_LookYaw = IA_LookYaw_Asset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_LookPitch_Asset(TEXT("/Game/GT/Inputs/IA_GT_LookPitch.IA_GT_LookPitch"));
	if (IA_LookPitch_Asset.Succeeded())
	{
		IA_LookPitch = IA_LookPitch_Asset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Jump_Asset(TEXT("/Game/GT/Inputs/IA_GT_Jump.IA_GT_Jump"));
	if (IA_Jump_Asset.Succeeded())
	{
		IA_Jump = IA_Jump_Asset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_AttackLB_Asset(TEXT("/Game/GT/Inputs/IA_GT_AttackLB.IA_GT_AttackLB"));
	if (IA_AttackLB_Asset.Succeeded())
	{
		IA_AttackLB = IA_AttackLB_Asset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_AttackRB_Asset(TEXT("/Game/GT/Inputs/IA_GT_AttackRB.IA_GT_AttackRB"));
	if (IA_AttackRB_Asset.Succeeded())
	{
		IA_AttackRB = IA_AttackRB_Asset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_E_Asset(TEXT("/Game/GT/Inputs/IA_GT_E.IA_GT_E"));
	if (IA_E_Asset.Succeeded())
	{
		IA_E = IA_E_Asset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_F_Asset(TEXT("/Game/GT/Inputs/IA_GT_F.IA_GT_F"));
	if (IA_F_Asset.Succeeded())
	{
		IA_F = IA_F_Asset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_R_Asset(TEXT("/Game/GT/Inputs/IA_GT_R.IA_GT_R"));
	if (IA_R_Asset.Succeeded())
	{
		IA_R = IA_R_Asset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_B_Asset(TEXT("/Game/GT/Inputs/IA_GT_B.IA_GT_B"));
	if (IA_B_Asset.Succeeded())
	{
		IA_B = IA_B_Asset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_H_Asset(TEXT("/Game/01_Common/IA_Key_H.IA_Key_H"));
	if (IA_H_Asset.Succeeded())
	{
		IA_H = IA_H_Asset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Shift_Asset(TEXT("/Game/GT/Inputs/IA_GT_Shift.IA_GT_Shift"));
	if (IA_Shift_Asset.Succeeded())
	{
		IA_Shift = IA_Shift_Asset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Ctrl_Asset(TEXT("/Game/GT/Inputs/IA_Ctrl.IA_Ctrl"));
	if (IA_Ctrl_Asset.Succeeded())
	{
		IA_Ctrl = IA_Ctrl_Asset.Object;
	}
}

void APlayerBase::MoveInput(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	if (!Controller) return;
	
	const FRotator ControlRot = Controller->GetControlRotation();
	const FRotator YawRot(0.f, ControlRot.Yaw, 0.f);
	
	const FVector Forward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
	const FVector Right	  = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(Forward, Axis.X);
	AddMovementInput(Right, Axis.Y);
}

void APlayerBase::LookYawInput(const FInputActionValue& Value)
{
	const float Axis = Value.Get<float>();
	AddControllerYawInput(Axis);
}

void APlayerBase::LookPitchInput(const FInputActionValue& Value)
{
	const float Axis = Value.Get<float>();
	AddControllerPitchInput(Axis);
}

void APlayerBase::JumpVelocityInput(float value)
{
	MoveComp->JumpZVelocity = value;
}

void APlayerBase::MoveSpeedInput(float speedValue, float accelValue)
{
	MoveComp->MaxWalkSpeed = speedValue;
	MoveComp->MaxAcceleration = accelValue;
}

void APlayerBase::MouseLBComplete_Implementation()
{
}

void APlayerBase::MouseLBTrigger_Implementation()
{
}

void APlayerBase::OnE_Implementation()
{
}

void APlayerBase::OnMouseRB_Implementation()
{
}

void APlayerBase::OnMouseLB_Implementation()
{
}

void APlayerBase::OnF_Implementation()
{
}

void APlayerBase::OnR_Implementation()
{
}

void APlayerBase::OnShift_Implementation()
{
}

void APlayerBase::OnH_Implementation()
{
	// 오버랩 된 상황에서 h 키를 누를 때 발생하는 내용 추가.
	// 공통적으로 사용되어야 함.
}

void APlayerBase::OnB_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnB"));
	if (IsChangeArmLength)
	{
		UE_LOG(LogTemp, Warning, TEXT("IsChangeArmLength : True"));
		return;
	}
	TogglePerspective();
}

void APlayerBase::StopShift_Implementation()
{
}

void APlayerBase::OnCtrl_Implementation()
{
	if (GetCharacterMovement() && GetCharacterMovement()->IsFalling())
	{
		return; // 공중이면 앉기 금지
	}

	Crouch();
}

void APlayerBase::StopCtrl_Implementation()
{
	UnCrouch();
}

void APlayerBase::SetPerspectiveMode(EPerspectiveMode NewMode)
{
	if ( PerspectiveMode == NewMode ) return;
	PerspectiveMode = NewMode;

	IsChangeArmLength = true;
}

FVector APlayerBase::GetForwardDir() const
{
	// 카메라 기준으로 계산
	auto vector = FPCamera->GetForwardVector();
	float distance = 300.f;
	return vector * distance;
}

// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Input Mapping
	APlayerController* PC = Cast<APlayerController>(Controller);
	if (!PC) return;
	ULocalPlayer* LP = PC->GetLocalPlayer();
	if (!LP) return;
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		if (InputMappingContext)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	
	APlayerCameraManager* CameraManager = GetLocalViewingPlayerController()->PlayerCameraManager;
	if (CameraManager)
	{
		CameraManager->ViewPitchMax = 90.0f;
		CameraManager->ViewPitchMin = -90.f;
	}
	
	// 최대 1단 점프까지 가능하도록 설정
	this->JumpMaxCount = 1;

}

// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// FPS - TPS 간 전환 시 카메라 이동 처리
	if (IsChangeArmLength)
	{
		float armLength = PerspectiveMode == EPerspectiveMode::FirstPerson ? 0.0f : THIRD_PERSON_ARM_LENGTH;
		float speed = SpringArm->TargetArmLength >= armLength ? 0 : 1000.0f;

		if (speed > 0.f)
		{
			SpringArm->TargetArmLength += DeltaTime * speed;
			if (SpringArm->TargetArmLength >= armLength) IsChangeArmLength = false;
		}
		else
		{
			SpringArm->TargetArmLength = armLength;
			IsChangeArmLength = false;
		}
	}
	
	if (bIsCrouched && GetCharacterMovement() && GetCharacterMovement()->IsFalling())
	{
		UnCrouch();
	}
	
}

// input관련된 
void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EIC) return;
	
	if (IA_Move) EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerBase::MoveInput);
	if (IA_LookYaw) EIC->BindAction(IA_LookYaw, ETriggerEvent::Triggered, this, &APlayerBase::LookYawInput);
	if (IA_LookPitch) EIC->BindAction(IA_LookPitch, ETriggerEvent::Triggered, this, &APlayerBase::LookPitchInput);

	if (IA_Jump)
	{
		EIC->BindAction(IA_Jump, ETriggerEvent::Started, this, &ACharacter::Jump);
		EIC->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	}
	
	EIC->BindAction(IA_AttackLB, ETriggerEvent::Started, this, &APlayerBase::OnMouseLB);
	EIC->BindAction(IA_AttackLB, ETriggerEvent::Triggered, this, &APlayerBase::MouseLBTrigger);
	EIC->BindAction(IA_AttackLB, ETriggerEvent::Completed, this, &APlayerBase::MouseLBComplete);
	EIC->BindAction(IA_AttackRB, ETriggerEvent::Started, this, &APlayerBase::OnMouseRB);
	EIC->BindAction(IA_E, ETriggerEvent::Started, this, &APlayerBase::OnE);
	EIC->BindAction(IA_F, ETriggerEvent::Started, this, &APlayerBase::OnF);
	EIC->BindAction(IA_R, ETriggerEvent::Started, this, &APlayerBase::OnR);
	EIC->BindAction(IA_B, ETriggerEvent::Started, this, &APlayerBase::OnB);
	EIC->BindAction(IA_H, ETriggerEvent::Started, this, &APlayerBase::OnH);
	EIC->BindAction(IA_Shift, ETriggerEvent::Started, this, &APlayerBase::OnShift);
	EIC->BindAction(IA_Shift, ETriggerEvent::Completed, this, &APlayerBase::StopShift);
	EIC->BindAction(IA_Ctrl, ETriggerEvent::Started, this, &APlayerBase::OnCtrl);
	EIC->BindAction(IA_Ctrl, ETriggerEvent::Completed, this, &APlayerBase::StopCtrl);
	
}

