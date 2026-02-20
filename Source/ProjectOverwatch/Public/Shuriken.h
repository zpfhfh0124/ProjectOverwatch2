// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Shuriken.generated.h"

class USphereComponent;
class APlayerBase;

UCLASS()
class PROJECTOVERWATCH_API AShuriken : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShuriken();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	TObjectPtr<USphereComponent> Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	TObjectPtr<USkeletalMeshComponent> SkMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float Damage = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float LifeSeconds = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shuriken")
	bool IsFiring = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shuriken")
	float SpinSpeed = 2000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shuriken")
	FName AttachedSocketName = "";

	UFUNCTION(BlueprintCallable, Category = "Shuriken")
	// 투척 (방향 및 속도 세팅)
	void FireInDirection(const FVector& ShootDir);

	// 회전
	void RotateInDirection(const FRotator& Rot);
	
	UFUNCTION(BlueprintCallable, Category = "Shuriken")
	// 장착된 소켓 네임 설정
	void SetSocketName(const FName& SocketName);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);
};



