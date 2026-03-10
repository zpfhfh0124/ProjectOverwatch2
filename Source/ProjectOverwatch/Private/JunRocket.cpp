// Fill out your copyright notice in the Description page of Project Settings.


#include "JunRocket.h"

#include "Engine/World.h"
#include "Engine/EngineTypes.h"

#include "JunCharacter.h"
#include "Components/SphereComponent.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/ProjectileMovementComponent.h"





// Sets default values
AJunRocket::AJunRocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	collisionComp = CreateDefaultSubobject<USphereComponent>("CollisionComp");
	collisionComp->SetGenerateOverlapEvents(true);
	collisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 또는 QueryAndPhysics
	collisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic")); // OverlapAll 말고 이게 더 안전
	collisionComp->SetSphereRadius(13);
	SetRootComponent(collisionComp);
	
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("BodyMesh");
	bodyMeshComp->SetupAttachment(collisionComp);
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bodyMeshComp->SetRelativeScale3D(FVector(0.25f));
	
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	movementComp->SetUpdatedComponent(collisionComp);
	
	movementComp->ProjectileGravityScale = 0.f;
	movementComp->InitialSpeed = 1500;
	movementComp->MaxSpeed = 1500;
	
	
}

// Called when the game starts or when spawned
void AJunRocket::BeginPlay()
{
	Super::BeginPlay();

	if (collisionComp)
	{
		collisionComp->OnComponentBeginOverlap.AddDynamic(this, &AJunRocket::OnMyCompBeginOverlab);
	}

	SetLifeSpan(10.f);

	//  발사 직후: Owner(발사자) 충돌/오버랩 완전 무시 시작
	bIgnoreOwnerCollision = true;

	if (AActor* OwnerActor = GetOwner())
	{
		if (collisionComp)
		{
			// 1) 이동(Sweep) 충돌에서 무시
			collisionComp->IgnoreActorWhenMoving(OwnerActor, true);
			collisionComp->MoveIgnoreActors.AddUnique(OwnerActor);

			// 2) (선택) Instigator도 같이 무시하고 싶으면
			// 보통 Owner=Instigator라 하나로 충분하지만 안전하게 같이 처리 가능
			if (AActor* Inst = GetInstigator())
			{
				collisionComp->IgnoreActorWhenMoving(Inst, true);
				collisionComp->MoveIgnoreActors.AddUnique(Inst);
			}
		}
	}

	//  일정 시간 뒤 무시 해제
	if (IgnoreOwnerSeconds > 0.f)
	{
		GetWorldTimerManager().SetTimer(
			IgnoreOwnerTimer,
			this,
			&AJunRocket::ClearIgnoreOwner,
			IgnoreOwnerSeconds,
			false
		);
	}
}

// Called every frame
void AJunRocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// P = P0 + velocity(direction * speed) * t
	Direction = GetActorForwardVector();
	FVector P0 = GetActorLocation();
	FVector velocity = Direction * movementComp->MaxSpeed;
	SetActorLocation(P0 + velocity * DeltaTime);
	
}

void AJunRocket::OnMyCompBeginOverlab(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
	//  발사 직후 일정 시간 동안은 "자기 자신"과의 오버랩은 무시
	if (bIgnoreOwnerCollision)
	{
		if (OtherActor && (OtherActor == GetOwner() || OtherActor == GetInstigator()))
		{
			return;
		} 
	}
	
	//  Destroy 먼저 하지 말고, 마지막에!
    const FVector Center = GetActorLocation(); // 폭심지(간단히 로켓 위치)

    const float Radius = 300.f;

    // FCollisionQueryParams Params(SCENE_QUERY_STAT(RocketExplosionOverlap), false);
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    //if (AActor* OwnerActor = GetOwner()) Params.AddIgnoredActor(OwnerActor);

    FCollisionObjectQueryParams ObjParams;
    ObjParams.AddObjectTypesToQuery(ECC_Pawn); //  Pawn만 수집
	

    TArray<FOverlapResult> Overlaps;
    const bool bAny = GetWorld()->OverlapMultiByObjectType(
        Overlaps,
        Center,
        FQuat::Identity,
        ObjParams,
        FCollisionShape::MakeSphere(Radius),
        Params
    );

    if (bAny)
    {
        for (const FOverlapResult& O : Overlaps)
        {
            APawn* Pawn = Cast<APawn>(O.GetActor());
            if (!Pawn) continue;

            // 폭심지 -> 대상 방향
            FVector Dir = (Pawn->GetActorLocation() - Center);
            Dir.Z = 0.f; // 수평 넉백만
            Dir = Dir.GetSafeNormal();

            // 솔져 느낌 튕김 값(튜닝)
            const float KnockbackSpeed = 300.f;
            const float UpBoost = 350.f;

            //  Character면 LaunchCharacter 가능
            if (ACharacter* Ch = Cast<ACharacter>(Pawn))
            {
                const FVector LaunchVel = Dir * KnockbackSpeed + FVector(0, 0, UpBoost);
                Ch->LaunchCharacter(LaunchVel, true, true);
            }
            else
            {
                //  Character가 아닌 Pawn은 LaunchCharacter 불가
                // (최소 대응) 루트가 물리 시뮬이면 임펄스로 밀기
                if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(Pawn->GetRootComponent()))
                {
                    if (Prim->IsSimulatingPhysics())
                    {
                        const float ImpulseStrength = 2000.f;
                        Prim->AddImpulse(Dir * ImpulseStrength, NAME_None, true);
                    }
                }
            }
        }
    }

    Destroy();
}

void AJunRocket::ClearIgnoreOwner()
{
	bIgnoreOwnerCollision = false;

	if (!collisionComp) return;

	if (AActor* OwnerActor = GetOwner())
	{
		collisionComp->IgnoreActorWhenMoving(OwnerActor, false);
		collisionComp->MoveIgnoreActors.Remove(OwnerActor);
	}

	if (AActor* Inst = GetInstigator())
	{
		collisionComp->IgnoreActorWhenMoving(Inst, false);
		collisionComp->MoveIgnoreActors.Remove(Inst);
	}
}