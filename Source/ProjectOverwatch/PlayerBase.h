// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectOverwatchCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerBase.generated.h"
#include "InputActionValue.h"

UENUM(BlueprintType)
enum class EPerspectiveMode : uint8
{
	FirstPerson,
	ThirdPerson
};

UCLASS()
class PROJECTOVERWATCH_API APlayerBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerBase();
	
	// components 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* FPCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* TPCamera;
	
	// Camera
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	EPerspectiveMode PerspectiveMode = EPerspectiveMode::FirstPerson;
	
	// Mesh
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	USkeletalMeshComponent* SkeletalMesh;
	
	// 카메라 시점 변환용 (토글)
	UFUNCTION(BlueprintCallable, Category="Input")
	void TogglePerspective(const FInputActionValue& value);			
	
	// 공용 입력 엔트리 (E스킬, F키, Shift ...) (BP/C++에서 Override 가능)
	UFUNCTION(BlueprintCallable, Category="Input")
	void OnMouseLB();
	virtual void OnMouseLB_Implementation();	// 일반 공격
	UFUNCTION(BlueprintCallable, Category="Input")
	void OnMouseRB();
	virtual void OnMouseRB_Implementation();	// 일반 공격 2
	UFUNCTION(BlueprintCallable, Category="Input")
	void OnE();
	virtual void OnE_Implementation();			// E 스킬
	UFUNCTION(BlueprintCallable, Category="Input")
	void OnF();
	virtual void OnF_Implementation();			// F 키 입력
	UFUNCTION(BlueprintCallable, Category="Input")
	void OnShift();
	virtual void OnShift_Implementation();		// 쉬프트 키 스킬

	// 카메라/에임
	UFUNCTION(BlueprintCallable, Category="Camera")
	void SetPerspectiveMode(EPerspectiveMode NewMode);
	UFUNCTION(BlueprintPure, Category="Camera")
	EPerspectiveMode GetPerspectiveMode() const { return PerspectiveMode; }
	UFUNCTION(BlueprintCallable, Category="Combat")
	void GetAimView(FVector& OutLocation, FRotator& OutRotation) const;
	
private:
	void ApplyPerspectiveVisibility();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
