// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "GameFramework/Actor.h"
#include "ShootManager.generated.h"

UCLASS()
class PVN_PROJECT_API AShootManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShootManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot")
	float MaxTravellingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot")
	TArray<float> LinesXLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot")
	TArray<FVector> ShootTargetLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot")
	TArray<FVector> MaxTravelLocations;

	UFUNCTION(BlueprintCallable, Category = "Shoot")
	int GetLineIndex(FVector SpawnLocation);

	UFUNCTION(BlueprintCallable, Category = "Shoot")
	USceneComponent* GetSceneComponent(AActor* MyActor);
	
	UFUNCTION(BlueprintCallable, Category = "Shoot")
	void ShootProjectile(AActor* Projectile, FVector SpawnLocation);

};
