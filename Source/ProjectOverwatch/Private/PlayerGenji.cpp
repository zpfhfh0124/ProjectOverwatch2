// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGenji.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

APlayerGenji::APlayerGenji()
{
	ReflectionBoxComp = CreateDefaultSubobject<UBoxComponent>("ReflectionBoxComp");
	ReflectionBoxComp->SetupAttachment(GetRootComponent());
	ReflectionBoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ReflectionBoxComp->SetRelativeLocation(FVector(50, 0, 0));
	ReflectionBoxComp->SetBoxExtent(FVector(50, 100, 100));
	ReflectionBoxComp->SetCollisionProfileName(TEXT("BlueWeapon"));
}

void APlayerGenji::BeginPlay()
{
	Super::BeginPlay();

	// 쿨타임 지정
	CountShiftCoolTime = 10.0f;
	CountECoolTime = 5.0f;
	
	InitIcons();
	
	// 팅겨내기 콜리전 충돌 설정
	SetReflectionBoxComp(false);
	ReflectionBoxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerGenji::OnHitReflection);
}

void APlayerGenji::InitIcons()
{
	ProjectileIconPath = "/Script/Paper2D.PaperSprite'/Game/GT/UI/Sprites/Projectile_Genji_Sprite_0.Projectile_Genji_Sprite_0'";
	SkillEIconPath = "/Script/Paper2D.PaperSprite'/Game/GT/UI/Sprites/Skill_Genji_Sprite_1.Skill_Genji_Sprite_1'";
	SkillShiftLIconPath = "/Script/Paper2D.PaperSprite'/Game/GT/UI/Sprites/Skill_Genji_Sprite_0.Skill_Genji_Sprite_0'";
	
	Super::InitIcons();
}

void APlayerGenji::StartDurationSkillE()
{
	SetReflectionBoxComp(true);
	
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APlayerGenji::EndDurationSkillE, EDuration, false);
}

void APlayerGenji::EndDurationSkillE_Implementation()
{
	SetReflectionBoxComp(false);
}

void APlayerGenji::SetReflectionBoxComp(bool Enable)
{
	if (ReflectionBoxComp == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ReflectionBoxComp is Null"));
		return;
	}
	
	ReflectionBoxComp->SetCollisionEnabled(Enable ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
}

void APlayerGenji::OnHitReflection(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 팅겨내기 반사 콜리전 충돌
	// 충돌체의 ProjectileMovementComponent 검사
	auto* otherPMComp = OtherActor->GetComponentByClass<UProjectileMovementComponent>();
	if (otherPMComp)
	{
		// 충돌체의 Velocity를 수정한다.
		// 플레이어가 향하고 있는 방향으로 교체
		FVector newVelocity = Controller->GetControlRotation().Vector().GetSafeNormal() * otherPMComp->InitialSpeed;
		otherPMComp->Velocity = newVelocity;

		// Debug
		/*DrawDebugSphere(
			GetWorld(),
			SweepResult.ImpactPoint,
			10.f,
			12,
			FColor::Green,
			false,
			2.f
		);*/
	}
}
