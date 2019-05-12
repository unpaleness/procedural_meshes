#include "HeavyPlane.h"

AHeavyPlane::AHeavyPlane() {}

void AHeavyPlane::GenerateMesh() {
	InnerVerticesPerSide = VerticesPerSide;

	Super::GenerateMesh();
}

bool AHeavyPlane::HasChanges() {
	bool result { false };
	if (Super::HasChanges()) {
		result = true;
	}
	if (VerticesPerSide != InnerVerticesPerSide) {
		result = true;
	}
	return result;
}

void AHeavyPlane::InitArrays() {
	Super::InitArrays();

	for (int32 k = 0; k < InnerVerticesPerSide * InnerVerticesPerSide; ++k) {
		int32 y { k / InnerVerticesPerSide };
		int32 x { k % InnerVerticesPerSide };
		Vertices.Add(FVector(-InnerSize / 2.0f + (InnerSize * x / (InnerVerticesPerSide - 1)), -InnerSize / 2.0f + (InnerSize * y / (InnerVerticesPerSide - 1)), 0.0f));
		UVs.Add(FVector2D(1.0f * x / (InnerVerticesPerSide - 1), 1.0f * (InnerVerticesPerSide - 1 - y) / (InnerVerticesPerSide - 1)));
		if (y < InnerVerticesPerSide - 1 && x < InnerVerticesPerSide - 1) {
			AddTriangle(k, k + InnerVerticesPerSide, k + InnerVerticesPerSide + 1);
			AddTriangle(k, k + InnerVerticesPerSide + 1, k + 1);
		}
	}
}
