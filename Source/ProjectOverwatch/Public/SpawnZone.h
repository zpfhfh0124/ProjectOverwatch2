// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnZone.generated.h"

UCLASS()
class PROJECTOVERWATCH_API ASpawnZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBoxComponent> SelectZone;

	// 스폰 존 진입
	UFUNCTION()
	void OnEnterSpawnZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						  int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// 스폰 존 이탈
	UFUNCTION()
	void OnExitSpawnZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
