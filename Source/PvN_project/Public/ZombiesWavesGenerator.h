// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/World.h"
#include "TimerManager.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Engine/SkeletalMesh.h"
#include "ZombiesWavesGenerator.generated.h"

UCLASS()
class PVN_PROJECT_API AZombiesWavesGenerator : public AActor
{
    GENERATED_BODY()

public:
    // Constructor 
    AZombiesWavesGenerator();

    // Declaration of the Tick function
    virtual void Tick(float DeltaTime) override;


    // UPROPERTIES : 
    // Arrays
    UPROPERTY(EditAnywhere, Category = "Zombie")
    TArray<FVector> SpawnPositions;

    UPROPERTY(EditAnywhere, Category = "Zombie")
    TArray<FVector> TargetPositions;

    UPROPERTY(EditAnywhere, Category = "Zombie")
    TArray<AActor*> ZombiesArray;

    UPROPERTY(EditAnywhere, Category = "Zombie")
    TArray<int> RandomIndex;


    // Subclasses
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zombie")
    TSubclassOf<AActor> ZombieClass;


    // Components
    UPROPERTY(EditAnywhere, Category = "Zombie")
    USkeletalMesh* ClassicZombieMesh;

    UPROPERTY(EditAnywhere, Category = "Zombie")
    USkeletalMesh* PlotZombieMesh;

    UPROPERTY(EditAnywhere, Category = "Zombie")
    USkeletalMesh* BucketZombieMesh;




    // UFUNCTIONS
    // Getters
    UFUNCTION(BlueprintCallable, Category = "Zombie")
    USkeletalMeshComponent* GetSkeletalMeshComponent(AActor* MyActor);

    UFUNCTION(BlueprintCallable, Category = "Zombie")
    USceneComponent* GetSceneComponent(AActor* MyActor);

    UFUNCTION(BlueprintCallable, Category = "Zombie")
    int GetRandomIndex();

    UFUNCTION(BlueprintCallable, Category = "Zombie")
    FVector GetRandomSpawnLocation(int IndexOfSpawning);


    // Setters
    UFUNCTION(BlueprintCallable, Category = "Zombie")
    void SetMeshToSkeletalMeshComponent(AActor* MyActor, USkeletalMesh* NewMesh);


    // Spawning, Starting, Stopping
    UFUNCTION(BlueprintCallable, Category = "Zombie")
    void InitRandomSeed();

    UFUNCTION(BlueprintCallable, Category = "Zombie")
    AActor* SpawnZombie(FVector SpawnLocation);

    UFUNCTION(BlueprintCallable, Category = "Zombie")
    void StopMovement(AActor* Zombie);

    UFUNCTION(BlueprintCallable, Category = "Zombie")
    void RestartMovement(AActor* Zombie);


    // Waves
    UFUNCTION(BlueprintCallable, Category = "Zombie")
    void StartWave1();

    UFUNCTION(BlueprintCallable, Category = "Zombie")
    void StartWave2();

    UFUNCTION(BlueprintCallable, Category = "Zombie")
    void StartWave3();

    UFUNCTION(BlueprintCallable, Category = "Zombie")
    void StartWave4();

    UFUNCTION(BlueprintCallable, Category = "Zombie")
    void StartWave5();

protected:
    // Declaration of the BeginPlay function
    virtual void BeginPlay() override;
};