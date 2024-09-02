// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"

#include "Components/CapsuleComponent.h"

#include "SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	if (GunClass)
    {
        Gun = GetWorld()->SpawnActor<AGun>(GunClass);
        if (Gun)
        {
            Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
            Gun->SetOwner(this);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to spawn Gun from GunClass!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("GunClass is not set!"));
    }
	
	/* //Original Code
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
	*/

	//WeaponSwitching//WeaponSwitching//WeaponSwitching
	for(TSubclassOf<AGun> WeaponClass : WeaponClasses)
	{
		AGun* SpawnedWeapon = GetWorld()->SpawnActor<AGun>(WeaponClass);
		if(SpawnedWeapon)
		{
			Weapons.Add(SpawnedWeapon);
			SpawnedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
			SpawnedWeapon->SetOwner(this);

			//Default Gun to index [0] -> Probably BP_Rifle
			for(int32 i = 0; i < Weapons.Num(); i++)
			{
				if(i == 0)
				{
					Weapons[i]->SetActorHiddenInGame(false);
				}
				else
				{
					Weapons[i]->SetActorHiddenInGame(true);
				}
			}
		}
	}
}	

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

///// AMMO
float AShooterCharacter::GetAmmo() const
{
	if(Gun)
	{
	return Gun->GetAmmoPercent();
	}
	return 0.0f;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

float AShooterCharacter::GetHealth() const
{
	return Health;
}

//Weapon Switching//Weapon Switching//Weapon Switching
void AShooterCharacter::UpdateWeaponVisibility()
{
	for(int32 i = 0; i < Weapons.Num(); i++)
	{
		if(i == ActiveIndex)
		{
			Weapons[i]->SetActorHiddenInGame(false);
			UE_LOG(LogTemp, Warning, TEXT("Weapon %d is now visible"), i);
		}
		else
		{
			Weapons[i]->SetActorHiddenInGame(true);
			UE_LOG(LogTemp, Warning, TEXT("Weapon %d is now hidden"), i);
		}
	}
}

//Weapon Switching//Weapon Switching//Weapon Switching
// 무기 전환 (축 입력 수정)
void AShooterCharacter::NextWeapon()
{
    if (Weapons.Num() > 0 ) // 값이 0이 아닌 경우에만 전환
    {

        ActiveIndex = (ActiveIndex + 1) % Weapons.Num();
        UpdateWeaponVisibility();
		Gun = Weapons[ActiveIndex];  // 현재 활성화된 무기로 설정
		UE_LOG(LogTemp, Warning, TEXT("NEXT WEAPON"));
		UE_LOG(LogTemp, Display, TEXT("ActiveIndex: %d"), ActiveIndex);
    }
}

// 무기 전환 (축 입력 수정)
void AShooterCharacter::PreviousWeapon()
{
    if (Weapons.Num() > 0 ) // 값이 0이 아닌 경우에만 전환
    {
		
        ActiveIndex = (ActiveIndex - 1 + Weapons.Num()) % Weapons.Num();
        UpdateWeaponVisibility();
		Gun = Weapons[ActiveIndex];  // 현재 활성화된 무기로 설정
		UE_LOG(LogTemp, Warning, TEXT("PREVIUOS WEAPON"));
		UE_LOG(LogTemp, Display, TEXT("ActiveIndex: %d"), ActiveIndex);
    }
}

FString AShooterCharacter::GetActiveWeaponName() const
{
	 // ActiveIndex가 유효한지 확인합니다.
    if (WeaponClasses.IsValidIndex(ActiveIndex))
    {
        TSubclassOf<AGun> WeaponClass = WeaponClasses[ActiveIndex];
        if (WeaponClass)
        {
            return WeaponClass->GetName();  // 유효한 경우 무기 클래스의 이름 반환
        }
    }

    return FString("Invalid Weapon");  // 유효하지 않은 경우 기본 문자열 반환
}









float AShooterCharacter::GetMaxHealth() const
{
	return MaxHealth;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;

	UE_LOG(LogTemp, Warning, TEXT("Character '%s' took %f damage. Health left: %f"), *GetName(), DamageToApply, Health);

	if(IsDead())
	{	
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
	
		if(GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}

		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		
	}

	return DamageToApply;

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//키보드, 마우스
	PlayerInputComponent->BindAxis(TEXT("MoveFoward"), this, &AShooterCharacter::MoveFoward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);



	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AShooterCharacter::AddControllerYawInput);


	//컨트롤러 XBox
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);


	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);


	// 스크롤 휠 바인딩 
    PlayerInputComponent->BindAction(TEXT("NextWeapon"), EInputEvent::IE_Pressed, this, &AShooterCharacter::NextWeapon);
    PlayerInputComponent->BindAction(TEXT("PreviousWeapon"), EInputEvent::IE_Pressed, this, &AShooterCharacter::PreviousWeapon);

    // 마우스 휠 축 바인딩 (이전 축 방식이 필요하다면)
}
// 키보드 마우스
void AShooterCharacter::MoveFoward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}



// 컨트롤러 
void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}


void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}


void AShooterCharacter::Shoot()
{
	if (Gun)  // Gun이 null인지 확인
    {
        Gun->PullTrigger();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Gun is not initialized!"));
    }
}

void AShooterCharacter::Heal(float Amount)
{
	if(Amount <= 0.f)
	{
		return;
	}
 
	Health = FMath::Clamp(Health + Amount, 0.f, MaxHealth);

	UE_LOG(LogTemp, Warning, TEXT("Health after healing %f"), Health);
}

// Ammo Recharge
void AShooterCharacter::EmmoRecharge(float Amount)
{

		Gun->RechargeAmmo(Amount);

	
}

float AShooterCharacter::GetPlayerAmmo()
{
	return Gun->GetAmmo();
}

float AShooterCharacter::GetPlayerMaxAmmo()
{
	return Gun->GetMaxAmmo();
}




