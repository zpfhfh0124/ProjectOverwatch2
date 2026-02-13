// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JunCharacter.generated.h"

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
	UPROPERTY(VisibleAnywhere, Category=Camera) class USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, Category=Camera) class UCameraComponent* FPSCamComp;
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputMappingContext* IMC_Jun;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* IA_JunLook;
	
	void look(const FInputActionValue& inputValue);

};
