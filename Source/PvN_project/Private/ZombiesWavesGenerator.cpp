
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
    AActor* ZombieSpawned = GetWorld()->SpawnActor<AActor>(ZombieClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
    ZombieSpawned->SetActorScale3D(FVector(0.4, 0.4, 0.4));
    USkeletalMeshComponent* ZombieSkeletalMeshComponent = GetSkeletalMeshComponent(ZombieSpawned);
    SetMeshToSkeletalMeshComponent(ZombieSpawned, ClassicZombieMesh);
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
        FLatentActionManager& LatentActionManager = GetWorld()->GetLatentActionManager();
        LatentActionManager.RemoveActionsForObject(Zombie);
    }
}

// Restart the movement of the zombie after being stopped
void AZombiesWavesGenerator::RestartMovement(AActor* Zombie)
{
    if (Zombie) {
        FLatentActionInfo LatentInfo;
        LatentInfo.CallbackTarget = Zombie;
        FVector TMPTargetPosition = FVector(Zombie->GetActorLocation().X, TargetPositions[RandomIndex[0]].Y, Zombie->GetActorLocation().Z);
        float remainingTime = (TargetPositions[RandomIndex[0]].Y - Zombie->GetActorLocation().Y) / (TargetPositions[RandomIndex[0]].Y - SpawnPositions[RandomIndex[0]].Y) * 30.0f;
        UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(Zombie), TMPTargetPosition, FRotator(0.0f, 0.0f, 0.0f), false, false, remainingTime, false, EMoveComponentAction::Type::Move, LatentInfo);
    }
}

// Used after spawning the zombies, start the wave 1 by adding the movement to the zombies
void AZombiesWavesGenerator::StartWave1()
{
    // Declaration of the variables used in the function only
    FLatentActionInfo LatentInfo1, LatentInfo2;

    // Set the CallbackTarget of the variables
    LatentInfo1.CallbackTarget = ZombiesArray[0];
    LatentInfo2.CallbackTarget = ZombiesArray[1];

    // Start the MoveComponentTo using the previous variables
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[0]), TargetPositions[RandomIndex[0]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo1);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[1]), TargetPositions[RandomIndex[1]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo2);
}

// Used after spawning the zombies, start the wave 2 by adding the movement to the zombies
void AZombiesWavesGenerator::StartWave2()
{
    // Declaration of the variables used in the function only
    FLatentActionInfo LatentInfo1, LatentInfo2, LatentInfo3;
    
    // Set the CallbackTarget of the variables
    LatentInfo1.CallbackTarget = ZombiesArray[2];
    LatentInfo2.CallbackTarget = ZombiesArray[3];
    LatentInfo3.CallbackTarget = ZombiesArray[4];

    // Start the MoveComponentTo using the previous variables
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[2]), TargetPositions[RandomIndex[2]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo1);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[3]), TargetPositions[RandomIndex[3]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo2); 
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[4]), TargetPositions[RandomIndex[4]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo3);
}

// Used after spawning the zombies, start the wave 3 by adding the movement to the zombies
void AZombiesWavesGenerator::StartWave3()
{
    // Declaration of the variables used in the function only
    FLatentActionInfo LatentInfo1, LatentInfo2, LatentInfo3, LatentInfo4, LatentInfo5, LatentInfo6;

    // Set the CallbackTarget of the variables
    LatentInfo1.CallbackTarget = ZombiesArray[5];
    LatentInfo2.CallbackTarget = ZombiesArray[6];
    LatentInfo3.CallbackTarget = ZombiesArray[7];
    LatentInfo4.CallbackTarget = ZombiesArray[8];
    LatentInfo5.CallbackTarget = ZombiesArray[9];
    LatentInfo6.CallbackTarget = ZombiesArray[10];

    // Start the MoveComponentTo using the previous variables
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[5]), TargetPositions[RandomIndex[5]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo1);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[6]), TargetPositions[RandomIndex[6]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo2);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[7]), TargetPositions[RandomIndex[7]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo3);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[8]), TargetPositions[RandomIndex[8]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo4);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[9]), TargetPositions[RandomIndex[9]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo5);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[10]), TargetPositions[RandomIndex[10]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo6);
}

// Used after spawning the zombies, start the wave 4 by adding the movement to the zombies
void AZombiesWavesGenerator::StartWave4()
{
    // Declaration of the variables used in the function only
    FLatentActionInfo LatentInfo1, LatentInfo2, LatentInfo3, LatentInfo4, LatentInfo5, LatentInfo6, LatentInfo7, LatentInfo8;

    // Set the CallbackTarget of the variables
    LatentInfo1.CallbackTarget = ZombiesArray[11];
    LatentInfo2.CallbackTarget = ZombiesArray[12];
    LatentInfo3.CallbackTarget = ZombiesArray[13];
    LatentInfo4.CallbackTarget = ZombiesArray[14];
    LatentInfo5.CallbackTarget = ZombiesArray[15];
    LatentInfo6.CallbackTarget = ZombiesArray[16];
    LatentInfo7.CallbackTarget = ZombiesArray[17];
    LatentInfo8.CallbackTarget = ZombiesArray[18];

    // Start the MoveComponentTo using the previous variables
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[11]), TargetPositions[RandomIndex[11]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo1);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[12]), TargetPositions[RandomIndex[12]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo2);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[13]), TargetPositions[RandomIndex[13]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo3);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[14]), TargetPositions[RandomIndex[14]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo4);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[15]), TargetPositions[RandomIndex[15]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo5);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[16]), TargetPositions[RandomIndex[16]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo6);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[17]), TargetPositions[RandomIndex[17]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo7);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[18]), TargetPositions[RandomIndex[18]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo8);
}

// Used after spawning the zombies, start the wave 5 by adding the movement to the zombies
void AZombiesWavesGenerator::StartWave5()
{
    // Declaration of the variables used in the function only
    FLatentActionInfo LatentInfo1, LatentInfo2, LatentInfo3, LatentInfo4, LatentInfo5, LatentInfo6, LatentInfo7, LatentInfo8, LatentInfo9, LatentInfo10, LatentInfo11;

    // Set the CallbackTarget of the variables
    LatentInfo1.CallbackTarget = ZombiesArray[19];
    LatentInfo2.CallbackTarget = ZombiesArray[20];
    LatentInfo3.CallbackTarget = ZombiesArray[21];
    LatentInfo4.CallbackTarget = ZombiesArray[22];
    LatentInfo5.CallbackTarget = ZombiesArray[23];
    LatentInfo6.CallbackTarget = ZombiesArray[24];
    LatentInfo7.CallbackTarget = ZombiesArray[25];
    LatentInfo8.CallbackTarget = ZombiesArray[26];
    LatentInfo9.CallbackTarget = ZombiesArray[27];
    LatentInfo10.CallbackTarget = ZombiesArray[28];
    LatentInfo11.CallbackTarget = ZombiesArray[29];

    // Start the MoveComponentTo using the previous variables
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[19]), TargetPositions[RandomIndex[19]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo1);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[20]), TargetPositions[RandomIndex[20]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo2);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[21]), TargetPositions[RandomIndex[21]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo3);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[22]), TargetPositions[RandomIndex[22]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo4);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[23]), TargetPositions[RandomIndex[23]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo5);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[24]), TargetPositions[RandomIndex[24]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo6);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[25]), TargetPositions[RandomIndex[25]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo7);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[26]), TargetPositions[RandomIndex[26]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo8);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[27]), TargetPositions[RandomIndex[27]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo9);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[28]), TargetPositions[RandomIndex[28]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo10);
    UKismetSystemLibrary::MoveComponentTo(GetSceneComponent(ZombiesArray[29]), TargetPositions[RandomIndex[29]], FRotator(0.0f, 0.0f, 0.0f), false, false, 30.0f, false, EMoveComponentAction::Type::Move, LatentInfo11);
}