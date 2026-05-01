// ============================================================================
// UNREAL ENGINE 6 - NANITE V2 (PURE COMPUTE CULLING)
// Target: RDNA 5 / DirectX 12 Ultimate (Shader Model 6.7+)
// ============================================================================

#pragma kernel ComputeCullAndLOD

// Structured buffers containing billions of micro-polygons
StructuredBuffer<float4> InstanceData : register(t0);
StructuredBuffer<uint> ClusterData : register(t1);

// Append buffer for the surviving visible polygons
AppendStructuredBuffer<uint> VisibleClustersOut : register(u0);

// Hardware-accelerated Depth Buffer for occlusion checking
Texture2D<float> HierarchicalZBuffer : register(t2);
SamplerState HZB_Sampler : register(s0);

cbuffer ViewConstants : register(b0) {
    matrix ViewProjection;
    float3 CameraPosition;
    float LOD_Threshold;
};

[numthreads(64, 1, 1)]
void ComputeCullAndLOD(uint3 DispatchThreadId : SV_DispatchThreadID) {
    uint clusterIndex = DispatchThreadId.x;
    
    // 1. Fetch the bounding sphere of the micro-polygon cluster
    float4 boundingSphere = InstanceData[clusterIndex];
    float3 center = boundingSphere.xyz;
    float radius = boundingSphere.w;

    // 2. Frustum Culling: Is it behind the camera or off-screen?
    float4 clipPos = mul(float4(center, 1.0), ViewProjection);
    if (clipPos.z < 0.0 || abs(clipPos.x) > clipPos.w + radius) {
        return; // Culled! Do not draw.
    }

    // 3. Neural LOD Selection
    // Instead of swapping models, Nanite dynamically adjusts the density mathematically
    float distanceToCam = distance(CameraPosition, center);
    if (distanceToCam > LOD_Threshold * 10.0) {
        return; // Too far, let the neural texture generator handle the background
    }

    // 4. Occlusion Culling: Is it hidden behind a wall?
    float2 screenUV = (clipPos.xy / clipPos.w) * 0.5 + 0.5;
    screenUV.y = 1.0 - screenUV.y;
    
    float depthInHZB = HierarchicalZBuffer.SampleLevel(HZB_Sampler, screenUV, 0);
    if (clipPos.z > depthInHZB + 0.001) {
        return; // Hidden by another object! Culled.
    }

    // 5. If it survived all checks, append it to the visible list to be rasterized
    VisibleClustersOut.Append(clusterIndex);
}
