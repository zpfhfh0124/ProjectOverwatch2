// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "PlayerBase.generated.h"

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
	//생성자
	APlayerBase();
	
	// components 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* FPCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* TPCamera;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UCharacterMovementComponent* MoveComp;
	
	// Camera
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	EPerspectiveMode PerspectiveMode = EPerspectiveMode::FirstPerson;

	bool IsChangeArmLength = false;
	
	// Mesh
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMesh;
	
	// 카메라 시점 변환용 (토글)
	UFUNCTION(BlueprintNativeEvent, Category="Input")
	void TogglePerspective();			
	
	// Input Assets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* IA_Move;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* IA_LookYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* IA_LookPitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* IA_Jump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* IA_AttackLB;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* IA_AttackRB;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* IA_E;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* IA_F;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* IA_R;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* IA_B;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* IA_H;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* IA_Shift;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* IA_Ctrl;

	void InitializeInputs();
	bool bIsShifting = false;
	
	// 플레이어 기본 Movement
	virtual void MoveInput(const FInputActionValue& Value);
	void LookYawInput(const FInputActionValue& Value);
	void LookPitchInput(const FInputActionValue& Value);
	void JumpVelocityInput(float value);
	
	// 공용 입력 엔트리 (E스킬, F키, Shift ...) (BP/C++에서 Override 가능)
	UFUNCTION(BlueprintNativeEvent, Category="Input")
	void OnMouseLB();	// 일반 공격
	UFUNCTION(BlueprintNativeEvent, Category="Input")
	void MouseLBTrigger();	// 일반 공격
	UFUNCTION(BlueprintNativeEvent, Category="Input")
	void MouseLBComplete();	// 일반 공격
	UFUNCTION(BlueprintNativeEvent, Category="Input")
	void OnMouseRB();	// 일반 공격 2
	UFUNCTION(BlueprintNativeEvent, Category="Input")
	void OnE();			// E 스킬
	UFUNCTION(BlueprintNativeEvent, Category="Input")
	void OnF();			// F 키 입력
	UFUNCTION(BlueprintNativeEvent, Category="Input")
	void OnR();			// R 키 입력
	UFUNCTION(BlueprintNativeEvent, Category="Input")
	void OnH();			// H 키 입력
	UFUNCTION(BlueprintNativeEvent, Category="Input")
	void OnB();			// B 키 입력
	UFUNCTION(BlueprintNativeEvent, Category="Input")
	void OnShift();		// 쉬프트 키 스킬
	UFUNCTION(BlueprintNativeEvent, Category="Input")
	void StopShift();
	UFUNCTION(BlueprintNativeEvent, Category="Input")
	void OnCtrl();
	UFUNCTION(BlueprintNativeEvent, Category="Input")
	void StopCtrl();
	
	// 카메라/에임
	UFUNCTION(BlueprintCallable, Category="Camera")
	void SetPerspectiveMode(EPerspectiveMode NewMode);
	UFUNCTION(BlueprintPure, Category="Camera")
	EPerspectiveMode GetPerspectiveMode() const { return PerspectiveMode; }
	UFUNCTION(BlueprintCallable, Category="Combat")
	FVector GetForwardDir() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	FTimerHandle ShiftTimerHandle;
	FTimerHandle ETimerHandle;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
