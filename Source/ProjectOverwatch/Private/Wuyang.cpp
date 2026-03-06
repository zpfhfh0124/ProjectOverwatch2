// Fill out your copyright notice in the Description page of Project Settings.


#include "Wuyang.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void AWuyang::BeginPlay()
{
	Super::BeginPlay();
}

AWuyang::AWuyang()
{
	//스폰애로우 생성
	SpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(FPCamera);
	SpawnPoint->ArrowSize = 1.0f;
	SpawnPoint->ArrowColor = FColor(255, 0, 0, 255);
	SpawnPoint->SetRelativeLocation(FVector(60.0f, 15.0f, -10.0f));
}

void AWuyang::OnMouseLB_Implementation()
{
	SpawnWaterBall();
}

void AWuyang::OnShift_Implementation()
{
	if (IsShiftSkillActive) return;
	//추후 생성 (이동 속도 강화 및 점프 사거리 추가)
	JumpVelocityInput(800.0f);
	IsShiftSkillActive = true;
	// n초 후 효력 정상화
	GetWorld()->GetTimerManager().SetTimer(
		ShiftTimerHandle, this,
		&AWuyang::ShiftCoolReset,
		4.0f, false
		);
}

void AWuyang::ShiftCoolReset()
{
	JumpVelocityInput(420.0f);
	IsShiftSkillActive = false;
}

void AWuyang::MouseLBComplete_Implementation()
{
	// 1. Is Valid 체크 (이미지의 ? Is Valid 노드)
	if (CurrentWaterBall && CurrentWaterBall->IsValidLowLevel())
	{
		// 2. Move Control 컴포넌트 찾기 (이미지의 Get Move Control 노드)
		// 보통 MoveControl은 ProjectileMovementComponent일 확률이 높습니다.
		UProjectileMovementComponent* moveComp = CurrentWaterBall->FindComponentByClass<UProjectileMovementComponent>();

		if (moveComp)
		{
			// 3. Gravity Scale 설정 (이미지의 SET Projectile Gravity Scale 노드)
			moveComp->ProjectileGravityScale = 1.0f; // 1.0으로 설정해서 중력 적용
            
			UE_LOG(LogTemp, Warning, TEXT("워터볼 중력 적용 완료!"));
		}
	}
    
	// 사용이 끝났으면 변수 초기화 (선택 사항)
	CurrentWaterBall = nullptr;
}

void AWuyang::MouseLBTrigger_Implementation()
{
	// 1. 워터볼이 있는지, 그리고 유효한지 체크
	if (!CurrentWaterBall) return;

	UProjectileMovementComponent* moveComp = CurrentWaterBall->FindComponentByClass<UProjectileMovementComponent>();
	// 이미지에서는 Sphere(컴포넌트)의 위치를 기준으로 계산하므로 가져옵니다.
	USceneComponent* SphereComp = Cast<USceneComponent>(CurrentWaterBall->GetComponentByClass(USphereComponent::StaticClass()));

	if (moveComp && SphereComp)
	{
		// 2. 카메라 정보 가져오기 (이미지의 Get Player Camera Manager 부분)
		if (!FPCamera) return;

		FVector CameraLoc = FPCamera->GetComponentLocation();
		FVector CameraForward = FPCamera->GetForwardVector();

		// 3. 목표 위치 계산 (카메라 앞 특정 지점)
		// 이미지의 복잡한 더하기 빼기 로직을 정리하면 결국 '카메라 앞 쪽에 유지'입니다.
		FVector TargetLocation = CameraForward * 300.0f;

		// 4. VInterp To (부드러운 위치 추적)
		FVector CurrentLoc = SphereComp->GetComponentLocation();
		
		float farLength = (CurrentLoc - CameraLoc).Length();
		FVector tempTargetLocation = CameraForward * farLength + CameraLoc;
		FVector interpVector = (tempTargetLocation - CurrentLoc) * 6.0f;
		float DeltaTime = GetWorld()->GetDeltaSeconds();
        
		// 이미지의 Interp Speed 150.0 반영
		FVector NextLocation = UKismetMathLibrary::VInterpTo(moveComp->Velocity, TargetLocation, DeltaTime, 150.0f);

		// 5. Velocity 설정 (이미지의 마지막 SET Velocity 부분)
		// 새로운 위치와 현재 위치의 차이를 이용해 속도를 계산하거나 직접 위치를 업데이트합니다.
		// 이미지에서는 Velocity를 직접 건드리므로:
		moveComp->Velocity = NextLocation + interpVector;
	}
}

void AWuyang::SpawnWaterBall()
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
			UProjectileMovementComponent* moveComp = CurrentWaterBall->FindComponentByClass<UProjectileMovementComponent>();

			if (moveComp)
			{
				// 3. 중력 스케일을 0으로 설정 (둥둥 떠다니게 함)
				moveComp->ProjectileGravityScale = 0.0f;
			}
		}
	}
}
