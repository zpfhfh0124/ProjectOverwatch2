// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JunCharacter.generated.h"


struct FInputActionValue;

UCLASS()
class PROJECTOVERWATCH_API AJunCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJunCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="camera") 
	class USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="camera") 
	class UCameraComponent* FPSCamComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AJunRocket> RocketFactory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* FirePoint;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TEnumAsByte<ECollisionChannel> AimTraceChannel = ECC_Visibility;

public:
	bool bIsShifting = false;
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputMappingContext* IMC_Jun;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* IA_JunLook;
	
	//마우스 인풋 시점
	void look(const FInputActionValue& inputValue);

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* IA_JunMove;
	//이동 속도
	UPROPERTY(EditDefaultsOnly, Category=PlayerSetting)
	//이동 방향
	FVector direction;
	
	void move(const struct FInputActionValue& inputValue);
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* IA_JunJump;
	//점프 입력 이벤트 처리 함수
	void jump(const struct FInputActionValue& inputValue);
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* IA_JunCrouch;
	//앉기 입력 이벤트 처리 함수
	void crouch(const struct FInputActionValue& inputValue);
	//앉기 끝
	void stopcrouch(const struct FInputActionValue& inputValue);
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* IA_JunShift;
	void shift(const struct FInputActionValue& inputValue);
	void stopshift(const struct FInputActionValue& inputValue);
	
	void PlayerMove();
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* IA_JunLeft;
	void left(const struct FInputActionValue& inputValue);

	

	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* IA_JunRight;
	void right(const struct FInputActionValue& inputValue);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UCharacterMovementComponent* MoveComp;
	
private:
	bool GetAimPointFromCamera(FVector& OutAimPoint, FVector& OutTraceEnd) const;
	
	
};
