// Fill out your copyright notice in the Description page of Project Settings.

#include "FlockingManager.h"
#include "Agent.h"


#define AGENT_COUNT 10

void UFlockingManager::Init(UWorld* world, UStaticMeshComponent* mesh) {
    UE_LOG(LogTemp, Warning, TEXT("Manager initialized"));

    World = world;
    float incr = (PI * 2.f) / AGENT_COUNT;
    for (int i = 0; i < AGENT_COUNT; i++) {
        if (World != nullptr) {
            FRotator rotation = FRotator();

            FVector location = FVector();
            location.X = FMath::Sin(incr * i) * 200.f;
            location.Z = FMath::Cos(incr * i) * 200.f;

            AAgent* agent = World->SpawnActor<AAgent>(location, rotation);
            agent->Init(mesh, i);
            Agents.Add(agent);
        }
    }

    initialized = true;
}

void UFlockingManager::Flock() {

    //THREE RULES

    FVector v1;
    FVector v2;
    FVector v3;
    FVector v4;
    AAgent* b;
    

   for (int i = 0; i < AGENT_COUNT; i++) {

            
       b = Agents[i];

       v1 = MoveTowardsFlockCenter(b);
       v2 = PersonalSpace(b);
       v3 = MatchSpeed(b);
       v4 = BoundPos(b);


       b->Velocity = b->Velocity + v1 + v2 + v3 + v4;
       //b.velocity = b.velocity + v1 + v2 + v3
       
       //Limit Speed
       LimitSpeed(b);
       
       //b->SetActorLocation(b->GetActorLocation() + b->Velocity);
       //b.position = b.position + b.velocity
      
   }


}


FVector UFlockingManager::MoveTowardsFlockCenter(AAgent* boid) {

    FVector CenterLocation = FVector(0.f);

    for (int i = 0; i < AGENT_COUNT; i++) {

        if (boid != Agents[i]) {

            CenterLocation = CenterLocation + Agents[i]->GetActorLocation();

        }

    }

    CenterLocation = CenterLocation / AGENT_COUNT - 1;

    return (CenterLocation - boid->GetActorLocation()) / 100;

}

FVector UFlockingManager::PersonalSpace(AAgent* boid) {

    FVector c = FVector(0.f);

    for (int i = 0; i < AGENT_COUNT; i++) {

        if (boid != Agents[i]) {

            //FVector locations = Agents[i]->GetActorLocation() - boid->GetActorLocation();
            float FLocation = FVector::Dist(Agents[i]->GetActorLocation(), boid->GetActorLocation());

            if (FMath::Abs(FLocation) < 100) {// || locations.Z < 10 || locations.Y < 10) {
                c = c - (Agents[i]->GetActorLocation() - boid->GetActorLocation());
            }

        }

    }

    return c;
}

FVector UFlockingManager::MatchSpeed(AAgent* boid) {

    FVector Velocity = FVector(0.f);

    for (int i = 0; i < AGENT_COUNT; i++) {

        if (boid != Agents[i]) {

            Velocity = Velocity + Agents[i]->GetVelocity();

        }

    }

    Velocity = Velocity / Agents.Num() - 1;

    return (Velocity - boid->GetVelocity()) / 8;


}

void UFlockingManager::LimitSpeed(AAgent* boid) {

    int16 Limit = 20;
    FVector v(0);


    if (boid->Velocity.GetAbs().Size() > Limit) {
        boid->Velocity = (boid->Velocity / boid->Velocity.GetAbs().Size()) * Limit;
    }


}


FVector UFlockingManager::BoundPos(AAgent* boid) {
    int16 Xmin = -1500;
    int16 Xmax = 1500;
    int16 Ymin = -1500;
    int16 Ymax = 1500;
    int16 Zmin = -1500;
    int16 Zmax = 1500;

    FVector v(0);

    if (boid->GetActorLocation().X < Xmin) {
        v.X = 100;
    }
    else if (boid->GetActorLocation().X > Xmax) {
        v.X = -100;
    }

    if (boid->GetActorLocation().X < Ymin) {
        v.Y = 100;
    }
    else if (boid->GetActorLocation().X > Ymax) {
        v.Y = -100;
    }

    if (boid->GetActorLocation().X < Zmin) {
        v.Z = 100;
    }
    else if (boid->GetActorLocation().X > Zmax) {
        v.Z = -100;
    }



    return v;
}
