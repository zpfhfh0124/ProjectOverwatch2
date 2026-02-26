// Fill out your copyright notice in the Description page of Project Settings.


#include "Katana.h"

#include "PlayerBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AKatana::AKatana()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// 충돌체 설정
	RootSceneComp = CreateDefaultSubobject<USceneComponent>("RootSceneComp");
	SetRootComponent(RootSceneComp);
	BoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComp");
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComp->SetGenerateOverlapEvents(true);
	
	// 메쉬 설정
	KatanaMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	KatanaMeshComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/GT/Genji/Katana/Genji_Katana.Genji_Katana'"));
	if (tempMesh.Succeeded())
	{
		KatanaMeshComp->SetStaticMesh(tempMesh.Object);
	}
}

// Called when the game starts or when spawned
void AKatana::BeginPlay()
{
	Super::BeginPlay();
	
	// 충돌 델리게이트 등록
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AKatana::OnKatanaHit);
}

// Called every frame
void AKatana::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKatana::SetOwnerPlayer(APlayerBase* Player)
{
	OwnerPlayer = Player;
}

void AKatana::OnKatanaHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                          int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 충돌체의 ProjectileMovementComponent 검사
	auto* otherPMComp = OtherActor->GetComponentByClass<UProjectileMovementComponent>();
	if (otherPMComp)
	{
		// 충돌체의 Velocity를 수정한다.
		// 소유 플레이어가 존재하면 플레이어가 향하고 있는 방향으로 교체 아니면 기존 방향의 반대 방향으로 교체
		FVector newVelocity = OwnerPlayer != nullptr ? 
							  OwnerPlayer->GetForwardDir().GetSafeNormal() * otherPMComp->InitialSpeed : 
							  otherPMComp->Velocity * -1; 
		otherPMComp->SetVelocityInLocalSpace(newVelocity);
	}
}

