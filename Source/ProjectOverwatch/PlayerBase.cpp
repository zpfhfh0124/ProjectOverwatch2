// Fill out your copyright notice in the Description page of Project Settings.

#define THIRD_PERSON_ARM_LENGTH 300.0f

#include "PlayerBase.h"
#include "Camera/CameraComponent.h"

// Sets default values
APlayerBase::APlayerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 0.0f;
}

void APlayerBase::TogglePerspective(const FInputActionValue& value)
{
	if (PerspectiveMode == EPerspectiveMode::FirstPerson) SetPerspectiveMode(EPerspectiveMode::ThirdPerson);
	else SetPerspectiveMode(EPerspectiveMode::FirstPerson);
}

void APlayerBase::SetPerspectiveMode(EPerspectiveMode NewMode)
{
	if ( PerspectiveMode == NewMode ) return;
	PerspectiveMode = NewMode;
	
	float armLength = PerspectiveMode == EPerspectiveMode::FirstPerson ? 0.0f : THIRD_PERSON_ARM_LENGTH;
	SpringArm->TargetArmLength = armLength;
	
	ApplyPerspectiveVisibility();
}

void APlayerBase::GetAimView(FVector& OutLocation, FRotator& OutRotation) const
{
}

void APlayerBase::ApplyPerspectiveVisibility()
{
	// FP : 손, 팔만 보이도록 몸은 숨김
	if (PerspectiveMode == EPerspectiveMode::FirstPerson)
	{
		GetMesh()->SetOwnerNoSee(true);
	}
	// TP : 전신 보이도록 설정
	else
	{
		GetMesh()->SetOwnerNoSee(false);
	}
}

// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();
	ApplyPerspectiveVisibility();
}

// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

