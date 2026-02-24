// Fill out your copyright notice in the Description page of Project Settings.


#include "JunCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "JunRocket.h"
#include "GameFramework/CharacterMovementComponent.h"



// Sets default values   
AJunCharacter::AJunCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	//스켈레탈 메시를 불러온다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Jun/Art/15151.15151'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		//Mesh 컴포넌트의 위치와 회전값을 설정하고 싶다.
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0, -90, 0));
	}
	// FPS 카메라를 붙이고 싶다.
	// 3-1 springArm 컴포넌트 붙이기
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeLocation(FVector(30.0f, 0.0f, 70.0f));
	SpringArmComponent->TargetArmLength = 10;
	SpringArmComponent->bUsePawnControlRotation = true;
	// 3-2 CameraComponent 붙이기
	FPSCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamComp"));
	FPSCamComp->SetupAttachment(SpringArmComponent);
	FPSCamComp->bUsePawnControlRotation = false;
	
	//카메라 마우스방향 조절?
	bUseControllerRotationYaw = true;
	//2단 점프
	JumpMaxCount = 2;
	
	//중력계쑤
	
	FirePoint = CreateDefaultSubobject<USceneComponent>("FirePoint");
	FirePoint->SetupAttachment(RootComponent);
	
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true; // 또는 bCanCrouch 설정
	// GetCharacterMovement()->CrouchedHalfHeight = 60.f; // 필요하면
	
	MoveComp = GetCharacterMovement();
	
	MoveComp->bCanWalkOffLedges = true;
	MoveComp->bCanWalkOffLedgesWhenCrouching = true;
	
	
}

// Called when the game starts or when spawned
void AJunCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//아래에 컨트롤러에 enhancedinputsystem 넣어주는거
	auto pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem)
		{
			subsystem->AddMappingContext(IMC_Jun, 0);
		}
		APlayerCameraManager* CameraManager = GetLocalViewingPlayerController()->PlayerCameraManager;
		if (CameraManager)
		{
			CameraManager->ViewPitchMax = 70.0f;
			CameraManager->ViewPitchMin = -70.f;
		}
	}
	
}

// Called every frame
void AJunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsCrouched && GetCharacterMovement() && GetCharacterMovement()->IsFalling())
	{
		UnCrouch();
	}

	
	PlayerMove();
}

// Called to bind functionality to input
void AJunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (PlayerInput)
	{
		PlayerInput->BindAction(IA_JunLook, ETriggerEvent::Triggered, this, &AJunCharacter::look);
		PlayerInput->BindAction(IA_JunMove, ETriggerEvent::Triggered, this, &AJunCharacter::move);
		PlayerInput->BindAction(IA_JunJump, ETriggerEvent::Triggered, this, &AJunCharacter::jump);
		PlayerInput->BindAction(IA_JunCrouch, ETriggerEvent::Started, this, &AJunCharacter::crouch);
		PlayerInput->BindAction(IA_JunCrouch, ETriggerEvent::Completed, this, &AJunCharacter::stopcrouch);
		PlayerInput->BindAction(IA_JunShift, ETriggerEvent::Started, this, &AJunCharacter::shift);
		PlayerInput->BindAction(IA_JunShift, ETriggerEvent::Completed, this, &AJunCharacter::stopshift);
		PlayerInput->BindAction(IA_JunLeft, ETriggerEvent::Triggered, this, &AJunCharacter::left);
		PlayerInput->BindAction(IA_JunRight, ETriggerEvent::Started, this, &AJunCharacter::right);

	}
}

void AJunCharacter::jump(const struct FInputActionValue& inputValue)
{
	Jump();
}

void AJunCharacter::crouch(const struct FInputActionValue& inputValue)
{
	if (GetCharacterMovement() && GetCharacterMovement()->IsFalling())
	{
		return; // 공중이면 앉기 금지
	}

	Crouch();
}

void AJunCharacter::stopcrouch(const struct FInputActionValue& inputValue)
{
	UnCrouch();
}

void AJunCharacter::move(const FInputActionValue& inputValue)
{
	if (!MoveComp) MoveComp = GetCharacterMovement();
	if (!MoveComp) return;

	const FVector2D Axis = inputValue.Get<FVector2D>();

	float Forward = Axis.X; // 전후 (W+, S-)
	float Right   = Axis.Y; // 좌우 (D+, A-)

	// 스프린트는 "Shift를 누르고 + 전진(Forward > 0)"일 때만
	const bool bSprintNow = bIsShifting && (Forward > 0.f);

	// 속도만 분기 (S는 걷기로 자연스럽게 후진)
	MoveComp->MaxWalkSpeed = bSprintNow ? 1200.f : 600.f;

	// 대각선 속도 이득 방지 (Forward/Right 그대로 클램프)
	FVector2D Clamped(Forward, Right);
	Clamped = Clamped.GetClampedToMaxSize(1.f);
	Forward = Clamped.X;
	Right   = Clamped.Y;

	// 컨트롤러 Yaw 기준 이동
	const float Yaw = Controller ? Controller->GetControlRotation().Yaw : GetActorRotation().Yaw;
	const FRotator YawRot(0.f, Yaw, 0.f);

	const FVector ForwardDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
	const FVector RightDir   = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDir, Forward);
	AddMovementInput(RightDir,   Right);
}

void AJunCharacter::look(const FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();
	AddControllerYawInput(value.X);
	AddControllerPitchInput(-value.Y);
}

void AJunCharacter::PlayerMove()
{
	//플레이어 이동 처리
	//등속 운동
	//P = P0 + vt
	direction = FTransform(GetActorRotation()).TransformVector(direction);
	/*FVector P0 = GetActorLocation();
	FVector vt = direction * walkSpeed * DeltaTime;
	FVector P = P0 + vt;
	SetActorLocation(P);*/
	AddMovementInput(direction);
	direction = FVector::ZeroVector;
}



void AJunCharacter::left(const struct FInputActionValue& inputValue)
{
	if (!FPSCamComp) return;

	const FVector Start = FPSCamComp->GetComponentLocation();
	const FVector End   = Start + (FPSCamComp->GetForwardVector() * 30000.f);

	FHitResult Hit;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(LeftTrace), true);
	Params.AddIgnoredActor(this);     // Ignore Self
	// Params.bTraceComplex = false;  // BP에서 Trace Complex 꺼져있으니 기본 false

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	// Draw Debug Type: For Duration 느낌
	const float Duration = 1.0f; // 원하는 시간으로 조절
	DrawDebugLine(GetWorld(), Start, bHit ? Hit.ImpactPoint : End, FColor::Green, false, Duration, 0, 1.5f);

	if (bHit)
	{
		DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 8.f, FColor::Red, false, Duration);
		// 여기서 Hit.GetActor(), Hit.BoneName 등으로 데미지/이펙트 처리 가능
	}
}

void AJunCharacter::right(const struct FInputActionValue& inputValue)
{
	FTransform t = FirePoint->GetComponentTransform();
	GetWorld()->SpawnActor<AJunRocket>(RocketFactory, t);
}

void AJunCharacter::shift(const struct FInputActionValue& inputValue)
{
	bIsShifting = true;
}

void AJunCharacter::stopshift(const struct FInputActionValue& inputValue)
{
	bIsShifting = false;
	MoveComp->MaxWalkSpeed = 600.f;
}