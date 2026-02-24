// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// 기본 콜리전 설정
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);
	SphereComponent->InitSphereRadius(8.f);
	//SphereComponent->SetCollisionProfileName(TEXT("Projectile"));
	SphereComponent->SetNotifyRigidBodyCollision(true);
	SphereComponent->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
	SphereComponent->SetEnableGravity(false);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	// 기본 메쉬 설정
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(SphereComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (tempMesh.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(tempMesh.Object);
	}
	
	// 메테리얼 설정
	ConstructorHelpers::FObjectFinder<UMaterial> tempMaterial(TEXT("/Script/Engine.Material'/Game/Weapons/GrenadeLauncher/Materials/M_ProjectileBullet.M_ProjectileBullet'"));
	if (tempMaterial.Succeeded())
	{
		StaticMeshComponent->SetMaterial(0, tempMaterial.Object);
	}
	
	// 투사체 설정
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = ProjectileMovement->InitialSpeed;
	ProjectileMovement->StopMovementImmediately();
	ProjectileMovement->SetUpdatedComponent(SphereComponent);
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->Velocity = GetActorForwardVector().GetSafeNormal() * ProjectileMovement->InitialSpeed;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->Activate(true);
	
	SetLifeSpan(LifeSeconds);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::OnHit_Implementation(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
}

