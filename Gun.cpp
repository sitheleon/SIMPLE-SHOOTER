// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

//언리얼 5.1이상은 include for FPointDamageEvent Struct.
#include "Engine/DamageEvents.h" 




// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

}

void AGun::PullTrigger()
{	
	UE_LOG(LogTemp, Warning, TEXT("GUN CLASS , TRIGGERED"));


	//Ammo
	if(Ammo < 1)
	{
		UGameplayStatics::SpawnSoundAttached(MuzzleEmptySound, Mesh, TEXT("MuzzleFlashSocket"));
		// Play empty sound
	}


	if(Ammo > 0){
		
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	Ammo -= 1;
	UE_LOG(LogTemp, Error, TEXT("Left Ammo %f"), Ammo);	
	
	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection);

	if(bSuccess)
	{	
		
		//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect , Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
		
		AActor* HitActor = Hit.GetActor();
		
		if(HitActor != nullptr)
		{	
			AController *OwnerController = GetOwnerController();
			//struct
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);

			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}			//shootercharacter.cpp에 있는 함수
	}
	
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	//Ammo
	Ammo = MaxAmmo;
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::DamageUp()
{
		Damage += 10.0f;
		UE_LOG(LogTemp, Warning , TEXT("Increased Gun Damage %f"), Damage);
	
}

//AMMO
float AGun::GetAmmoPercent() const
{
	return Ammo/MaxAmmo;

}



// Ammo Recharge
void AGun::RechargeAmmo(float Amount)
{	
	if(Ammo < MaxAmmo)
	{	//Min은 두 값 비교하여 더 적은 값 반환. (Ammo + Amount가 MaxAmmo 초과 시  더 적은 값인 MaxAmmo리턴. )
		// 결론적으로 Ammo를 아무리 더 먹어도 MaxAmmo값이 되기 때문.
		Ammo = FMath::Min(Ammo + Amount, MaxAmmo);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(" Already MAX AMMO"));
	}
}




bool AGun::GunTrace(FHitResult &Hit, FVector ShotDirection)
{
	AController *OwnerController = GetOwnerController();

	if(OwnerController == nullptr)
	{
		return false;
	}

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	
	ShotDirection = -Rotation.Vector();
	FVector End = Location + Rotation.Vector() * MaxRange;
	// 끝 점 계산

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

 	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);



}


AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if(OwnerPawn == nullptr)
	{
		return nullptr;
	}

	return OwnerPawn->GetController();

	
}

