// ============================================================================
// UNREAL ENGINE 6 - NEURAL PATH TRACING
// ============================================================================

#include "RayTracingHelpers.hlsl" // Conceptually included UE6 header

RaytracingAccelerationStructure SceneBVH : register(t0);
Texture2D<float4> GBufferAlbedo : register(t1);
Texture2D<float4> GBufferNormals : register(t2);

RWTexture2D<float4> NoisyRadianceOutput : register(u0);

[shader("raygeneration")]
void NeuralPathTracer_RayGen() {
    uint2 pixelCoord = DispatchRaysIndex().xy;
    float2 resolution = DispatchRaysDimensions().xy;
    
    float3 worldPosition, worldNormal, albedo;
    GetGBufferData(pixelCoord, worldPosition, worldNormal, albedo);

    // Instead of firing 32+ rays for lighting, we fire exactly ONE.
    // We rely on the XDNA Neural Denoising hardware to fill in the gaps.
    
    RayDesc ray;
    ray.Origin = worldPosition + worldNormal * 0.001f; // Offset to prevent self-shadowing
    ray.Direction = CosineSampleHemisphere(worldNormal, RandomNumberGenerator(pixelCoord));
    ray.TMin = 0.01f;
    ray.TMax = 10000.0f;

    RayIntersection payload;
    payload.Radiance = float3(0, 0, 0);
    
    // Trace the single bounce
    TraceRay(SceneBVH, RAY_FLAG_NONE, ~0, 0, 1, 0, ray, payload);

    // Combine the light hit with the surface color
    float3 finalNoisyColor = payload.Radiance * albedo;

    // Output to the buffer. The screen will look like television static at this stage.
    // The C++ NeuralRenderer engine will intercept this buffer and AI-denoise it.
    NoisyRadianceOutput[pixelCoord] = float4(finalNoisyColor, 1.0);
}
