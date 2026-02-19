// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_WuYang.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"

APlayer_WuYang::APlayer_WuYang()
{
	SpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(FirstPersonCameraComponent);
	
	SpawnPoint->ArrowSize = 1.0f;
	SpawnPoint->ArrowColor = FColor(255, 0, 0, 255);
	SpawnPoint->SetRelativeLocation(FVector(60.0f, 15.0f, -10.0f));
	
}

void APlayer_WuYang::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//LB
		EnhancedInputComponent->BindAction(MouseLBAction, ETriggerEvent::Started, this, &APlayer_WuYang::MouseLBStart);
		EnhancedInputComponent->BindAction(MouseLBAction, ETriggerEvent::Triggered, this, &APlayer_WuYang::MouseLBTrigger);
		EnhancedInputComponent->BindAction(MouseLBAction, ETriggerEvent::Completed, this, &APlayer_WuYang::MouseLBComplete);
	}
	else
	{
	}
}

void APlayer_WuYang::MouseLBStart()
{
	SpawnWaterBall();
}

void APlayer_WuYang::MouseLBTrigger()
{
	
}

void APlayer_WuYang::MouseLBComplete()
{
	
}

void APlayer_WuYang::SpawnWaterBall()
{
	UWorld* World = GetWorld();
	if (World && WaterBallClass)
	{
		FTransform SpawnTransform = SpawnPoint->GetComponentTransform();
		// 3. 스폰 옵션 설정 (소유자 설정 및 충돌 처리 방식)
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		// 1. 워터볼 소환
		AActor* SpawnedActor = World->SpawnActor<AActor>(WaterBallClass, SpawnTransform, SpawnParams);

		if (SpawnedActor)
		{
			// 2. 워터볼 내부에서 ProjectileMovementComponent(MoveControl)를 찾습니다.
			// 클래스 이름이 'MoveControl'로 되어 있어도 기본 타입이나 부모 타입을 넣으면 찾아집니다.
			UProjectileMovementComponent* MoveComp = SpawnedActor->FindComponentByClass<UProjectileMovementComponent>();

			if (MoveComp)
			{
				// 3. 중력 스케일을 0으로 설정 (둥둥 떠다니게 함)
				MoveComp->ProjectileGravityScale = 0.0f;
                
				// 만약 속도가 멈춰있다면 강제로 발사 방향으로 속도를 줄 수도 있습니다.
				// MoveComp->Velocity = SpawnTransform.GetRotation().GetForwardVector() * MoveComp->InitialSpeed;
                
				UE_LOG(LogTemp, Warning, TEXT("WaterBall Gravity Scale set to 0!"));
			}
		}
	}
}