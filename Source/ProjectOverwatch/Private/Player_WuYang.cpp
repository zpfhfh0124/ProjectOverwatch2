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
		//EnhancedInputComponent->BindAction(MouseLBAction, ETriggerEvent::Triggered, this, &APlayer_WuYang::MouseLBTrigger);
		EnhancedInputComponent->BindAction(MouseLBAction, ETriggerEvent::Completed, this, &APlayer_WuYang::MouseLBComplete);
	}
	else
	{
	}
}

void APlayer_WuYang::MouseLBStart(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("=== 마우스클릭 함수 시작 ==="));
	SpawnWaterBall();
}

void APlayer_WuYang::MouseLBTrigger(const FInputActionValue& Value)
{
	
}

void APlayer_WuYang::MouseLBComplete(const FInputActionValue& Value)
{
	// 1. Is Valid 체크 (이미지의 ? Is Valid 노드)
	if (CurrentWaterBall && CurrentWaterBall->IsValidLowLevel())
	{
		// 2. Move Control 컴포넌트 찾기 (이미지의 Get Move Control 노드)
		// 보통 MoveControl은 ProjectileMovementComponent일 확률이 높습니다.
		UProjectileMovementComponent* MoveComp = CurrentWaterBall->FindComponentByClass<UProjectileMovementComponent>();

		if (MoveComp)
		{
			// 3. Gravity Scale 설정 (이미지의 SET Projectile Gravity Scale 노드)
			MoveComp->ProjectileGravityScale = 1.0f; // 1.0으로 설정해서 중력 적용
            
			UE_LOG(LogTemp, Warning, TEXT("워터볼 중력 적용 완료!"));
		}
	}
    
	// 사용이 끝났으면 변수 초기화 (선택 사항)
	CurrentWaterBall = nullptr;
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
		CurrentWaterBall = World->SpawnActor<AActor>(WaterBallClass, SpawnTransform, SpawnParams);

		if (CurrentWaterBall)
		{
			// 2. 워터볼 내부에서 ProjectileMovementComponent(MoveControl)를 찾습니다.
			// 클래스 이름이 'MoveControl'로 되어 있어도 기본 타입이나 부모 타입을 넣으면 찾아집니다.
			UProjectileMovementComponent* MoveComp = CurrentWaterBall->FindComponentByClass<UProjectileMovementComponent>();

			if (MoveComp)
			{
				// 3. 중력 스케일을 0으로 설정 (둥둥 떠다니게 함)
				MoveComp->ProjectileGravityScale = 0.0f;
			}
		}
	}
}