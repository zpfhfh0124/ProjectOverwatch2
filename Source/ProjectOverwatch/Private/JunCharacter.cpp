// Fill out your copyright notice in the Description page of Project Settings.


#include "JunCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
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
	
	bUseControllerRotationYaw = true;
	//2단 점프
	JumpMaxCount = 2;
	
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true; // 또는 bCanCrouch 설정
	// GetCharacterMovement()->CrouchedHalfHeight = 60.f; // 필요하면
	
	auto* MoveComp = GetCharacterMovement();
	
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

void AJunCharacter::move(const struct FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();
	//상하 입력 이벤트 처리
	direction.X = value.X;
	//좌우 입력
	direction.Y = value.Y;
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