
#include "DefeatZone.h"
#include "UObject/ConstructorHelpers.h"

ADefeatZone::ADefeatZone()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Configuration de la zone de collision
    CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponenSt"));
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ADefeatZone::OnOverlapBegin);
    RootComponent = CollisionComponent;

    // Configuration de la classe de widget
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/YourWidgetClassPath/YourWidgetClass"));
    WidgetClass = WidgetClassFinder.Class;
}

void ADefeatZone::BeginPlay()
{
    Super::BeginPlay();
}

void ADefeatZone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ADefeatZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Création et affichage du widget
    if (WidgetClass)
    {
        UUserWidget* YourWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);

        if (YourWidget)
        {
            YourWidget->AddToViewport();
        }
    }
}