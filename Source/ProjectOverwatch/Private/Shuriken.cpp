// Fill out your copyright notice in the Description page of Project Settings.


#include "Shuriken.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AShuriken::AShuriken()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Collision
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);
	Collision->InitSphereRadius(8.f);
	Collision->SetCollisionProfileName(TEXT("Projectile"));
	Collision->SetNotifyRigidBodyCollision(true);
	Collision->OnComponentHit.AddDynamic(this, &AShuriken::OnHit);
	Collision->SetEnableGravity(false);

	// Mesh
	SkMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkMesh->SetupAttachment(Collision);
	SkMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkMesh->SetSimulatePhysics(false);
	SkMesh->SetEnableGravity(false);

	// Projectile Movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = Collision;
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->MaxSpeed = 2000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AShuriken::FireInDirection(const FVector& ShootDir)
{
	IsFiring = true;
	ProjectileMovement->Velocity = ShootDir * ProjectileMovement->InitialSpeed;
	ProjectileMovement->Activate();
	// 소멸 시한 설정 (투척시에만)
	SetLifeSpan(LifeSeconds);
}

void AShuriken::RotateInDirection(const FRotator& Rot)
{
	SkMesh->AddLocalRotation(Rot);
}

void AShuriken::OnHit(UPrimitiveComponent* HitComp,
                      AActor* OtherActor,
                      UPrimitiveComponent* OtherComp,
                      FVector NormalImpulse,
                      const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this) return;

	// 데미지 전달
	AActor* InstigatorActor = GetInstigator();
	AController* InstigatorController = InstigatorActor ? InstigatorActor->GetInstigatorController() : nullptr;

	UGameplayStatics::ApplyPointDamage(
		OtherActor,
		Damage,
		GetVelocity().GetSafeNormal(),
		Hit,
		InstigatorController,
		this,
		nullptr);

	Destroy();
}

// Called when the game starts or when spawned
void AShuriken::BeginPlay()
{
	Super::BeginPlay();
	IsFiring = false;
}

// Called every frame
void AShuriken::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsFiring)
	{
		// 회전
		RotateInDirection(FRotator(0.f, SpinSpeed * DeltaTime, 0.f));
	}
}

