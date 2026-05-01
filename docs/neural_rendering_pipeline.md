# Neural Rendering Pipeline: Nanite V2 & Lumen AI

**Status:** UE6 Core Architecture
**Topic:** The Death of the Rasterizer and the Rise of the NPU

## Nanite V2: Pure Compute
UE5 introduced Nanite, which used a mix of hardware rasterization and compute shaders to draw billions of triangles. UE6 abandons traditional rasterization almost entirely. Nanite V2 is a 100% GPU-Compute-driven pipeline. It analyzes the Hierarchical Z-Buffer (HZB) and culls invisible micro-polygons in microseconds. If a polygon is behind a wall, the GPU doesn't even know it exists.

## Neural Path Tracing
True path tracing (like in Hollywood CGI) requires bouncing hundreds of light rays per pixel. Real-time path tracing has traditionally crippled frame rates. 

UE6 solves this via **Neural Denoising**. The engine fires exactly *one* ray per pixel. This results in a lighting buffer that looks like television static. This buffer is immediately sent to the dedicated Neural Processing Unit (NPU), where a locally run AI model hallucinates the missing light bounces based on temporal history, outputting a flawless, noiseless 4K image with zero performance hit to the main GPU cores.

## Generative Textures
Game install sizes are exceeding 150GB. UE6 drastically reduces this footprint by storing "Latent Noise Maps" on the SSD. These tiny files are expanded into 8K textures in real-time by the NPU directly inside VRAM, meaning a 150GB game now fits into a 30GB footprint.
