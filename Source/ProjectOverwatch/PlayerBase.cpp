// Fill out your copyright notice in the Description page of Project Settings.

#define THIRD_PERSON_ARM_LENGTH 300.0f

#include "PlayerBase.h"

#include "InputMappingContext.h"
#include "InputState.h"

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
}

void APlayerBase::TogglePerspective(const FInputActionValue& value)
{
	if (PerspectiveMode == EPerspectiveMode::FirstPerson) SetPerspectiveMode(EPerspectiveMode::ThirdPerson);
	else SetPerspectiveMode(EPerspectiveMode::FirstPerson);
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

void APlayerBase::SetPerspectiveMode(EPerspectiveMode NewMode)
{
	if ( PerspectiveMode == NewMode ) return;
	PerspectiveMode = NewMode;
	
	float armLength = PerspectiveMode == EPerspectiveMode::FirstPerson ? 0.0f : THIRD_PERSON_ARM_LENGTH;
	SpringArm->TargetArmLength = armLength;
	
	ApplyPerspectiveVisibility();
}

FVector APlayerBase::GetAimCrosshair() const
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return GetActorForwardVector();

	int32 SizeX, SizeY;
	PC->GetViewportSize(SizeX, SizeY);

	FVector WorldOrigin;
	FVector WorldDirection;

	PC->DeprojectScreenPositionToWorld(SizeX * 0.5f, SizeY * 0.5f, WorldOrigin, WorldDirection);

	FVector End = WorldOrigin + WorldDirection * 100000.f;

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, WorldOrigin, End, ECC_Visibility);

	FVector TargetPoint = Hit.bBlockingHit ? Hit.ImpactPoint : End;
	return TargetPoint;
}

void APlayerBase::ApplyPerspectiveVisibility()
{
	// FP : 손, 팔만 보이도록 몸은 숨김
	if (PerspectiveMode == EPerspectiveMode::FirstPerson)
	{
		//GetMesh()->SetOwnerNoSee(true);
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
	EIC->BindAction(IA_AttackRB, ETriggerEvent::Started, this, &APlayerBase::OnMouseRB);
	EIC->BindAction(IA_E, ETriggerEvent::Started, this, &APlayerBase::OnE);
	EIC->BindAction(IA_F, ETriggerEvent::Started, this, &APlayerBase::OnF);
	EIC->BindAction(IA_R, ETriggerEvent::Started, this, &APlayerBase::OnR);
	EIC->BindAction(IA_Shift, ETriggerEvent::Started, this, &APlayerBase::OnShift);
}

