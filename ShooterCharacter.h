// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	/////AMMO / HUD
	UFUNCTION(BlueprintPure)
	float GetAmmo() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure)
	float GetHealth() const;

	//Weapon Switching//Weapon Switching//Weapon Switching
	UPROPERTY()
	TArray<AGun*> Weapons;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TArray<TSubclassOf<AGun>> WeaponClasses;

	//Weapon Switching//Weapon Switching//Weapon Switching
	int32 ActiveIndex;

	//Weapon Switching//Weapon Switching//Weapon Switching
	void UpdateWeaponVisibility();

	//Weapon Switching//Weapon Switching//Weapon Switching
	void NextWeapon();	
	void PreviousWeapon();

	UFUNCTION(BlueprintCallable, Category = "Combat")
    FString GetActiveWeaponName() const;






	
	UFUNCTION(BlueprintPure)
	float GetMaxHealth() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Actor.h에서 오버라이딩
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Shoot();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Emmo")
	void EmmoRecharge(float Amount);

	UPROPERTY(BlueprintReadOnly)
	AGun* Gun;

	UFUNCTION(BlueprintCallable, Category = "AmmoFunctions")
	float GetPlayerAmmo();

	UFUNCTION(BlueprintCallable, Category = "AmmoFunctions")
	float GetPlayerMaxAmmo();




private:


	void MoveFoward(float AxisValue);

	void MoveRight(float AxisValue);

	void LookUpRate(float AxisValue);

	void LookRightRate(float AxisValue);


	


	UPROPERTY(EditAnywhere)
	float RotationRate = 10;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;


	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere)	
	float Health;

	
};
