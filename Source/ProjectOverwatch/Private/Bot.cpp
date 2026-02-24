// Fill out your copyright notice in the Description page of Project Settings.


#include "Bot.h"

#include "Bullet.h"
#include "Components/BoxComponent.h"

class ABullet;
// Sets default values
ABot::ABot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("BoxComponent"));
	SetRootComponent(BoxComponent);
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMesh.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(tempMesh.Object);
	}
	
	ConstructorHelpers::FObjectFinder<UMaterial> tempMat(TEXT("/Script/Engine.Material'/DatasmithContent/Materials/FBXImporter/DeltaGenReference.DeltaGenReference'"));
	if (tempMat.Succeeded())
	{
		StaticMeshComponent->SetMaterial(0, tempMat.Object);
	}
	
	FirePoint = CreateDefaultSubobject<USceneComponent>(FName("FirePoint"));
	FirePoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrFireTime += DeltaTime;
	if (CurrFireTime >= FireDelayTime)
	{
		FireBulletForward();
		CurrFireTime = 0.0f;
	}
}

void ABot::FireBulletForward()
{
	// Bullet 생성
	GetWorld()->SpawnActor<ABullet>(BulletFactory, FirePoint->GetComponentTransform());
}

