// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JunRocket.generated.h"

UCLASS()
class PROJECTOVERWATCH_API AJunRocket : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJunRocket();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//발사체의 이동 담당 컴포
	UPROPERTY(EditDefaultsOnly, Category = Movement)
	class UProjectileMovementComponent* movementComp;
	//발사체 콜리전
	UPROPERTY(EditDefaultsOnly, Category = Collision)
	class USphereComponent* collisionComp;
	//발사체 외관
	UPROPERTY(EditDefaultsOnly, Category = BodyMesh)
	class UStaticMeshComponent* bodyMeshComp;
	
	
	FVector Direction;
	
};
