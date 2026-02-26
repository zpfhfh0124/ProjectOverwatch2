// Fill out your copyright notice in the Description page of Project Settings.


#include "JunRocket.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AJunRocket::AJunRocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collisionComp = CreateDefaultSubobject<USphereComponent>("CollisionComp");
	collisionComp->SetCollisionProfileName("OverlapAll");
	collisionComp->SetSphereRadius(13);
	SetRootComponent(collisionComp);
	
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("BodyMesh");
	bodyMeshComp->SetupAttachment(collisionComp);
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bodyMeshComp->SetRelativeScale3D(FVector(0.25f));
	
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	movementComp->SetUpdatedComponent(collisionComp);
	
	movementComp->InitialSpeed = 2500;
	movementComp->MaxSpeed = 2500;
	
}

// Called when the game starts or when spawned
void AJunRocket::BeginPlay()
{
	Super::BeginPlay();
	
	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &AJunRocket::OnMyCompBeginOverlab);
	
	SetLifeSpan(10.f);
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

void AJunRocket::OnMyCompBeginOverlab(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	this->Destroy();
	if (this->Destroy())
	{
		
	}
}

