#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "Base.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogProceduralMeshes, Log, All)

UCLASS()
class PROCEDURALMESHES_API ABase : public AActor {
	GENERATED_BODY()

public:
	ABase();
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

	virtual void GenerateMesh();
	virtual void CheckChanges();
	virtual void InitArrays();
	virtual void InitVertices();
	void AddTriangle(int32 V1, int32 V2, int32 V3);

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
		UProceduralMeshComponent* Mesh;

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector2D> UVs;
};
