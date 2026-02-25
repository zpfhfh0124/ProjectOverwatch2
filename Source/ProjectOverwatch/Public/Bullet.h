// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Bullet.generated.h"

UCLASS()
class PROJECTOVERWATCH_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Speed = 1000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LifeSeconds = 5.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsFiring = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="컴포넌트")
	TObjectPtr<USphereComponent> SphereComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="컴포넌트")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="컴포넌트")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	// 충돌
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);
	
};
