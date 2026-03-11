// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnZone.h"

#include "PlayerHUD.h"
#include "ProjectOverwatchPlayerController.h"
#include "Components/BoxComponent.h"
#include "Components/SlateWrapperTypes.h"

// Sets default values
ASpawnZone::ASpawnZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SelectZone = CreateDefaultSubobject<UBoxComponent>(FName("SpawnZoneComp"));
	SetRootComponent(SelectZone);
	SelectZone->SetWorldScale3D(FVector(10.f, 10.f, 0.5f));
	SelectZone->SetCollisionProfileName(FName("SpawnZone"));
	SelectZone->SetBoxExtent(FVector(50.f, 50.f, 50.f));
}

// Called when the game starts or when spawned
void ASpawnZone::BeginPlay()
{
	Super::BeginPlay();
	
	// 스폰존 오버랩 델리게이트
	SelectZone->OnComponentBeginOverlap.AddDynamic(this, &ASpawnZone::OnEnterSpawnZone);
	SelectZone->OnComponentEndOverlap.AddDynamic(this, &ASpawnZone::OnExitSpawnZone);
}

// Called every frame
void ASpawnZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnZone::OnEnterSpawnZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto* PC = Cast<AProjectOverwatchPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->GetPlayerHUD()->SetBtnPlayerChange(true);
	}
}

void ASpawnZone::OnExitSpawnZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto* PC = Cast<AProjectOverwatchPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->GetPlayerHUD()->SetBtnPlayerChange(false);
	}
}

