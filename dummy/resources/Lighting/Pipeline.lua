createTexture(SingleCubeMap, "Skybox", {512}, {format=RGBA16F, minFilter=LinearMipMapNearest, magFilter=Linear})
linkTexture(Single2D, "BRDFLUT", {minFilter=LinearMipMapNearest, magFilter=Linear, wrap=ClampToEdge})
createTexture(Single3D, "CloudDensity", {512, 8, 512}, {format=RGBA16F, minFilter=Linear, magFilter=Linear, wrap=Repeat, mipmaps=true})
createTexture(Single3D, "CloudNoise", {64, 64, 64}, {format=RGBA16F, minFilter=Linear, magFilter=Linear, wrap=Repeat, mipmaps=true})

gbuffer = createRenderTarget(screen.width, screen.height, {"AlbedoRough", "PosDepth", "NormalMetal", "Emission"}, {format=RGBA16F, minFilter=LinearMipMapNearest, magFilter=Linear, depthBuffer=true})
clouds = createRenderTarget(screen.width / 2, screen.height / 2, {"Clouds"}, {format=RGBA16F})
finalHDR = createRenderTarget(screen.width, screen.height, {"HDRFrame"}, {format=RGBA16F})
final = createRenderTarget(screen.width, screen.height, {"FinalTex"}, {format=RGB8})
setFinalRenderTarget(final, 0)

skyboxShader = findShader("Skybox")
cloudsRenderShader = findShader("CloudsRender")
lightingComputeShader = findShader("LightingCompute")
tonemappingShader = findShader("Tonemapping")

dispatchCompute(findShader("GenerateClouds"), 16, 8, 16)
dispatchCompute(findShader("GenerateCloudNoise"), 2, 64, 2)

function render ()
	dispatchCompute(skyboxShader, 16, 16, 6)
	drawScreen(clouds, cloudsRenderShader)
	clearRenderTarget(gbuffer, {renderTargetBuffer.color, renderTargetBuffer.depth})
	drawScene(gbuffer, {"PBR"}, {})
	drawScreen(finalHDR, lightingComputeShader)
	drawScreen(final, tonemappingShader)
end

function resize ()
end
