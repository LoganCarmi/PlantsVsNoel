
#include "ZombiesWavesGenerator.h"


// Sets default values
AZombiesWavesGenerator::AZombiesWavesGenerator()
{
    PrimaryActorTick.bCanEverTick = true;
}

// BeginPlay function called when the game starts or when spawned
void AZombiesWavesGenerator::BeginPlay()
{
    Super::BeginPlay();
}

// Tick function called every frame
void AZombiesWavesGenerator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Get SkeletalMeshComponent from Actor
USkeletalMeshComponent* AZombiesWavesGenerator::GetSkeletalMeshComponent(AActor* MyActor)
{
    if (MyActor)
    {
        // Check if MyActor have a SkeletalMeshComponent by casting to it
        USkeletalMeshComponent* SkeletalMeshComp = Cast<USkeletalMeshComponent>(MyActor->GetComponentByClass(USkeletalMeshComponent::StaticClass()));

        if (SkeletalMeshComp)
        { 
            return SkeletalMeshComp;                                // MyActor does have a SkeletalMeshComponent
        }
        else
        {
            return nullptr;                                         // MyActor doesn't have any SkeletalMeshComponent
        }
    }

    return nullptr;
}

// Get SceneComponent of Actor if exists
USceneComponent* AZombiesWavesGenerator::GetSceneComponent(AActor* MyActor)
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

// Set Mesh to the SkeletalMesh of the Actor passed as parameter
void AZombiesWavesGenerator::SetMeshToSkeletalMeshComponent(AActor* MyActor, USkeletalMesh* NewMesh)
{
    USkeletalMeshComponent* SkeletalMeshComp = GetSkeletalMeshComponent(MyActor);

    if (SkeletalMeshComp && NewMesh)
    {
        SkeletalMeshComp->SetSkeletalMesh(NewMesh);
    }
}

// Spawn Zombie at selected location, Set SkelatalMesh, add to Zombies array
AActor* AZombiesWavesGenerator::SpawnZombie(FVector SpawnLocation)
{
    FActorSpawnParameters SpawnParams;
    AActor* ZombieSpawned = GetWorld()->SpawnActor<AActor>(ZombieClass, SpawnLocation, FRotator(0.0f, 180.0f, 0.0f), SpawnParams);
    ZombieSpawned->SetActorScale3D(FVector(0.2, 0.2, 0.2));
    USkeletalMeshComponent* ZombieSkeletalMeshComponent = GetSkeletalMeshComponent(ZombieSpawned);
    SetMeshToSkeletalMeshComponent(ZombieSpawned, ClassicZombieMesh);
    if (ZombieWalkAnim) {
        ZombieSkeletalMeshComponent->PlayAnimation(ZombieWalkAnim, true);
    }
    ZombiesArray.Add(ZombieSpawned);
    return ZombieSpawned;
}

// Initialize Function using Seed for Random Generation
void AZombiesWavesGenerator::InitRandomSeed()
{
    FMath::RandInit(FPlatformTime::Cycles64());                                 // Using current time as a Seed for the generator
}

// Generate a random number, add it to the RandomIndex array and returns it
int AZombiesWavesGenerator::GetRandomIndex()
{
    int RandomIndexGenerated = FMath::RandRange(0, SpawnPositions.Num() - 1);   // Generate random number
    RandomIndex.Add(RandomIndexGenerated);                                      // Add the random number generated to the RandomIndex array
    return RandomIndexGenerated;                                                // return the generated number
}

// Get the location from the SpawnPositions at the selected index
FVector AZombiesWavesGenerator::GetRandomSpawnLocation(int IndexOfSpawning)
{
    return SpawnPositions[IndexOfSpawning];
}

// Stop the movement of the zombie
void AZombiesWavesGenerator::StopMovement(AActor* Zombie) 
{
    if (Zombie) {
        GetSkeletalMeshComponent(Zombie)->Stop();
        FLatentActionManager& LatentActionManager = GetWorld()->GetLatentActionManager();
        LatentActionManager.RemoveActionsForObject(Zombie);
    }
}

// Restart the movement of the zombie after being stopped
void AZombiesWavesGenerator::RestartMovement(AActor* Zombie)
{
    if (Zombie) {
        GetSkeletalMeshComponent(Zombie)->PlayAnimation(ZombieWalkAnim, true);
        FLatentActionInfo LatentInfo;
        LatentInfo.CallbackTarget = Zombie;
        FVector TMPTargetPosition = FVector(Zombie->GetActorLocation().X, TargetPositions[RandomIndex[0]].Y, Zombie->GetActorLocation().Z);
        float remainingTime = (TargetPositions[RandomIndex[0]].Y - Zombie->GetActorLocation().Y) / (TargetPositions[RandomIndex[0]].Y - SpawnPositions[RandomIndex[0]].Y) * 30.0f;
        UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(Zombie), TMPTargetPosition, FRotator(0.0f, 180.0f, 0.0f), false, false, remainingTime, false, EMoveComponentAction::Type::Move, LatentInfo);
    }
}

void AZombiesWavesGenerator::SpawnWave(int32 WaveIndex) {
    FLatentActionInfo LatentInfo;
    int32 MinAmount = ParametersPerWave[WaveIndex].MinAmountZombies;
    int32 MaxAmount = ParametersPerWave[WaveIndex].MaxAmountZombies;

    int32 RandomAmount = FMath::RandRange(MinAmount, MaxAmount);

    for (int32 i = 0; i < RandomAmount; i++)
    {
        int RandomIndexGenerated = GetRandomIndex();
        FVector RandomLocation = GetRandomSpawnLocation(RandomIndexGenerated);
        SpawnZombie(RandomLocation);
        AmountZombieSpawned++;

        LatentInfo.CallbackTarget = ZombiesArray[AmountZombieSpawned - 1];
        UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[AmountZombieSpawned - 1]), TargetPositions[RandomIndex[AmountZombieSpawned - 1]], FRotator(0.0f, 180.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo);
    }
}

void AZombiesWavesGenerator::SpawnEndlessWave() {
    FLatentActionInfo LatentInfo;
    EndlessWaveCounter++;
    int32 MinAmount = EndlessRangeWave.MinAmountZombies + (EndlessWaveCounter * IncreasingRatioEndless);
    int32 MaxAmount = EndlessRangeWave.MaxAmountZombies + (EndlessWaveCounter * IncreasingRatioEndless);

    int32 RandomAmount = FMath::RandRange(MinAmount, MaxAmount);

    for (int32 i = 0; i < RandomAmount; i++)
    {
        int RandomIndexGenerated = GetRandomIndex();
        FVector RandomLocation = GetRandomSpawnLocation(RandomIndexGenerated);
        SpawnZombie(RandomLocation);
        AmountZombieSpawned++;

        LatentInfo.CallbackTarget = ZombiesArray[AmountZombieSpawned - 1];
        UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[AmountZombieSpawned - 1]), TargetPositions[RandomIndex[AmountZombieSpawned - 1]], FRotator(0.0f, 180.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo);
    }
}