#include "Cube.h"
#include "Kismet/KismetMaterialLibrary.h"

ACube::ACube() {
	InitArrays();
}

void ACube::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ACube::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
}

void ACube::BeginPlay() {
	Super::BeginPlay();
}

void ACube::GenerateMesh() {
	InnerSize = Size;

	Super::GenerateMesh();
}

void ACube::CheckChanges() {
	if (Size != InnerSize) {
		GenerateMesh();
	}
}

void ACube::InitArrays() {
	AddTriangle(0, 3, 1);
	AddTriangle(0, 2, 3);
	AddTriangle(4, 7, 5);
	AddTriangle(4, 6, 7);
	AddTriangle(8, 11, 9);
	AddTriangle(8, 10, 11);
	AddTriangle(12, 15, 13);
	AddTriangle(12, 14, 15);
	AddTriangle(16, 19, 17);
	AddTriangle(16, 18, 19);
	AddTriangle(20, 23, 21);
	AddTriangle(20, 22, 23);

	for (int i = 0; i < 6; ++i) {
		UVs.Add(FVector2D(0.0f, 1.0f));
		UVs.Add(FVector2D(0.0f, 0.0f));
		UVs.Add(FVector2D(1.0f, 1.0f));
		UVs.Add(FVector2D(1.0f, 0.0f));
	}
}

void ACube::InitVertices() {
	Super::InitVertices();

	FVector V0(-InnerSize / 2.0f, -InnerSize / 2.0f, -InnerSize / 2.0f);
	FVector V1(-InnerSize / 2.0f, -InnerSize / 2.0f, InnerSize / 2.0f);
	FVector V2(-InnerSize / 2.0f, InnerSize / 2.0f, -InnerSize / 2.0f);
	FVector V3(-InnerSize / 2.0f, InnerSize / 2.0f, InnerSize / 2.0f);
	FVector V4(InnerSize / 2.0f, -InnerSize / 2.0f, -InnerSize / 2.0f);
	FVector V5(InnerSize / 2.0f, -InnerSize / 2.0f, InnerSize / 2.0f);
	FVector V6(InnerSize / 2.0f, InnerSize / 2.0f, -InnerSize / 2.0f);
	FVector V7(InnerSize / 2.0f, InnerSize / 2.0f, InnerSize / 2.0f);

	Vertices.Add(V0);
	Vertices.Add(V1);
	Vertices.Add(V2);
	Vertices.Add(V3);
	Vertices.Add(V2);
	Vertices.Add(V3);
	Vertices.Add(V6);
	Vertices.Add(V7);
	Vertices.Add(V6);
	Vertices.Add(V7);
	Vertices.Add(V4);
	Vertices.Add(V5);
	Vertices.Add(V4);
	Vertices.Add(V5);
	Vertices.Add(V0);
	Vertices.Add(V1);
	Vertices.Add(V0);
	Vertices.Add(V2);
	Vertices.Add(V4);
	Vertices.Add(V6);
	Vertices.Add(V3);
	Vertices.Add(V1);
	Vertices.Add(V7);
	Vertices.Add(V5);
}
