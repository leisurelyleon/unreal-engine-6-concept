using UnrealBuildTool;

public class NeuralRendererV2 : ModuleRules
{
    public NeuralRendererV2(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        // Enforce extreme optimization for the rendering thread
        bUseUnity = false;
        OptimizeCode = CodeOptimization.Always;

        // Public dependencies required to talk to the core engine and the RHI (Render Hardware Interface)
        PublicDependencyModuleNames.AddRange(new string[] { 
            "Core", 
            "CoreUObject", 
            "Engine", 
            "RenderCore", 
            "RHI" 
        });

        // Private dependencies for the specific neural hardware APIs
        PrivateDependencyModuleNames.AddRange(new string[] {
            "DirectX12_Ultimate", // Conceptual DX12 Agility SDK integration
            "TensorCoreInference" // Conceptual UE6 NPU access library
        });

        // Include paths for our custom shaders
        PublicIncludePaths.Add("neural_renderer_v2/src/shaders");

        System.Console.WriteLine("[UBT] Neural Renderer V2 Build configured. AI Upscaling and Nanite Compute active.");
    }
}
