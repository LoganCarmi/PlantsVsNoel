#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "DefeatZone.generated.h"

UCLASS()
class PVN_PROJECT_API ADefeatZone : public AActor
{
    GENERATED_BODY()

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Sets default values for this actor's properties
    ADefeatZone();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "DefeatZone")
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UPROPERTY(VisibleAnywhere)
    UBoxComponent* CollisionComponent;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> WidgetClass;
};