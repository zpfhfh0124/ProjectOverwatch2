// Fill out your copyright notice in the Description page of Project Settings.


#include "Shuriken.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AShuriken::AShuriken()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Collision 초기 설정 (장착중에는 콜리전 off)
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);
	Collision->InitSphereRadius(8.f);
	Collision->SetCollisionProfileName(TEXT("Projectile"));
	Collision->SetNotifyRigidBodyCollision(true);
	Collision->OnComponentHit.AddDynamic(this, &AShuriken::OnHit);
	Collision->SetEnableGravity(false);
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Mesh
	SkMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkMesh->SetupAttachment(Collision);
	SkMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkMesh->SetSimulatePhysics(false);
	SkMesh->SetEnableGravity(false);

	// Projectile Movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = Collision;
	ProjectileMovement->InitialSpeed = 2000.0f;
	ProjectileMovement->MaxSpeed = ProjectileMovement->InitialSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AShuriken::FireInDirection(const FVector& ShootDir)
{
	IsFiring = true;

	// 소켓 고정 해제
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// 충돌 설정
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// 소유자 캐릭터 Mesh 충돌 무시
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		Collision->IgnoreActorWhenMoving(OwnerCharacter, true);
		if (OwnerCharacter->GetMesh())
			Collision->IgnoreComponentWhenMoving(OwnerCharacter->GetMesh(), true);
	}

	// 투사체 설정
	ProjectileMovement->StopMovementImmediately();
	ProjectileMovement->SetUpdatedComponent(Collision);
	//ProjectileMovement->ProjectileGravityScale = 0.2f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->Velocity = ShootDir.GetSafeNormal() * ProjectileMovement->InitialSpeed;
	ProjectileMovement->Activate(true);

	// 소멸 시한 설정 (투척시에만)
	SetLifeSpan(LifeSeconds);
}

void AShuriken::RotateInDirection(const FRotator& Rot)
{
	SkMesh->AddLocalRotation(Rot);
}

void AShuriken::SetSocketName(const FName& SocketName)
{
	AttachedSocketName = SocketName;
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

	PrevLocation = GetActorLocation();
}

// Called every frame
void AShuriken::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsFiring)
	{
		// 회전 YZX 
		RotateInDirection(FRotator(SpinSpeed * DeltaTime, 0.f, 0.f));
		FString name = GetActorNameOrLabel();
		FVector vector = GetActorLocation();
		//UE_LOG(LogTemp, Warning, TEXT("%s Location : (%.2f, %.2f, %.2f)"), *name, vector.X, vector.Y, vector.Z);

		/// 디버깅 ------------
		FVector Current = GetActorLocation();

		DrawDebugLine(
			GetWorld(),
			PrevLocation,
			Current,
			FColor::Cyan,
			false,
			5.f,
			0,
			1.5f
		);

		PrevLocation = Current;
		/// -------------------
	}
}

