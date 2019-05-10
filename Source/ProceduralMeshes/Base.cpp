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
	InitArrays();

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

bool ABase::HasChanges() {
	return false;
}

void ABase::InitArrays() {
	Vertices.Empty();
	Triangles.Empty();
	UVs.Empty();
}
