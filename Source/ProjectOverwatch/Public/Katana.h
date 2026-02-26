// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Katana.generated.h"

UCLASS()
class PROJECTOVERWATCH_API AKatana : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKatana();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class USceneComponent* RootSceneComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UStaticMeshComponent* KatanaMeshComp;
	
	// 소유한 플레이어의 정보를 저장할 포인터
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APlayerBase* OwnerPlayer;
	
	UFUNCTION(BlueprintCallable)
	void SetOwnerPlayer(APlayerBase* Player);
	
	// 충돌 델리게이트
	UFUNCTION()
	void OnKatanaHit(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult);
};
