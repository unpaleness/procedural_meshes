#include "Base.h"
#include "Kismet/KismetMaterialLibrary.h"

DEFINE_LOG_CATEGORY(LogProceduralMeshes)

ABase::ABase() {
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->bUseAsyncCooking = true;

	SetRootComponent(Mesh);
}

void ABase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ABase::OnConstruction(const FTransform& Transform) {
	CheckChanges();
}

void ABase::BeginPlay() {
	Super::BeginPlay();

	CheckChanges();
}

void ABase::GenerateMesh() {
	InitVertices();

	UE_LOG(LogProceduralMeshes, Display, TEXT("%s: Start generating mesh"), *GetName());
	UE_LOG(LogProceduralMeshes, Display, TEXT("%s: Vertices: %i"), *GetName(), Vertices.Num());
	UE_LOG(LogProceduralMeshes, Display, TEXT("%s: Triangles: %i"), *GetName(), Triangles.Num() / 3);
	UE_LOG(LogProceduralMeshes, Display, TEXT("%s: UVs: %i"), *GetName(), UVs.Num());

	if (IsValid(Mesh)) {
		if (Mesh->GetNumSections() == 0) {
			Mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
		}
		else {
			Mesh->UpdateMeshSection_LinearColor(0, Vertices, TArray<FVector>(), UVs, TArray<FVector2D>(), TArray<FVector2D>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>());
		}
	}
	else {
		UE_LOG(LogProceduralMeshes, Warning, TEXT("%s: Mesh is invalid!"), *GetName());
	}
}

void ABase::AddTriangle(int32 V1, int32 V2, int32 V3) {
	Triangles.Add(V1);
	Triangles.Add(V2);
	Triangles.Add(V3);
}

void ABase::CheckChanges() {}

void ABase::InitArrays() {}

void ABase::InitVertices() {
	Vertices.Empty();
}
