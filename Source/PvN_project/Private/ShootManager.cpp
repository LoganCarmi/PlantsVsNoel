// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootManager.h"

// Sets default values
AShootManager::AShootManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShootManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShootManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

USceneComponent* AShootManager::GetSceneComponent(AActor* MyActor)
{
    if (MyActor)
    {
        // Check if MyActor have a SceneComponent by casting to it
        USceneComponent* SceneComponent = Cast<USceneComponent>(MyActor->GetComponentByClass(USceneComponent::StaticClass()));

        if (SceneComponent)
        {
            return SceneComponent;                                  // MyActor does have a SceneComponent
        }
        else
        {
            return nullptr;                                         // MyActor doesn't have any SceneComponent
        }
    }
    return nullptr;
}

int AShootManager::GetLineIndex(FVector SpawnLocation) {
    float XPosition = SpawnLocation.X;
    for (int i = 0; i < LinesXLocation.Num(); i++)
    {
        if (LinesXLocation[i] == XPosition) {
            return i;
        }
    }
    return 0;
}

void AShootManager::ShootProjectile(AActor* Projectile, FVector SpawnLocation) {
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = Projectile;
    
    int LineIndex = GetLineIndex(SpawnLocation);
    FVector TargetLocation = ShootTargetLocations[LineIndex];
    
    float OverTime = (TargetLocation.Y - SpawnLocation.Y) / (TargetLocation.Y - MaxTravelLocations[LineIndex].Y) * MaxTravellingTime;

    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(Projectile), TargetLocation, FRotator(0.0f, 180.0f, 0.0f), false, false, OverTime, false, EMoveComponentAction::Type::Move, LatentInfo);
}
