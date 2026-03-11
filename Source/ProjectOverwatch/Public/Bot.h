// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bot.generated.h"

UCLASS()
class PROJECTOVERWATCH_API ABot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 박스 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "컴포넌트")
	TObjectPtr<class UBoxComponent> BoxComponent;
	
	// 스태틱 메쉬 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "컴포넌트")
	TObjectPtr<class UStaticMeshComponent> StaticMeshComponent;
	
	// 투사체 스폰 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "컴포넌트")
	TObjectPtr<class UArrowComponent> FirePointA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "컴포넌트")
	TObjectPtr<class UArrowComponent> FirePointB;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ABC")
	bool IsFireModel = true;
	
	// 투사체 생성
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABullet> BulletFactory;
	
	// 투사 시간
	float FireDelayTime = 1.f;
	float CurrFireTime = 0.f;
	
	// 투사체 발사
	UFUNCTION(Blueprintable)
	void FireBulletForward();
};
