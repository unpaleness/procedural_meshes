#include "Base.h"

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
	if (HasChanges()) {
		GenerateMesh();
	}
}

void ABase::BeginPlay() {
	Super::BeginPlay();

	if (HasChanges()) {
		GenerateMesh();
	}
}

void ABase::GenerateMesh() {
	InnerSize = Size;

	InitArrays();

	Mesh->ClearMeshSection(0);
	Mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
}

void ABase::AddTriangle(int32 V1, int32 V2, int32 V3) {
	Triangles.Add(V1);
	Triangles.Add(V2);
	Triangles.Add(V3);
}

bool ABase::HasChanges() {
	if (Size != InnerSize) {
		return true;
	}
	return false;
}

void ABase::InitArrays() {
	Vertices.Empty();
	Triangles.Empty();
	UVs.Empty();
}
