// Fill out your copyright notice in the Description page of Project Settings.


#include "JunCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

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
}

// Called when the game starts or when spawned
void AJunCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

}

// Called to bind functionality to input
void AJunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (PlayerInput)
	{
		PlayerInput->BindAction(IA_JunLook, ETriggerEvent::Triggered, this, &AJunCharacter::look);
	}
}

void AJunCharacter::look(const FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();
	AddControllerYawInput(value.X);
	AddControllerPitchInput(-value.Y);
}