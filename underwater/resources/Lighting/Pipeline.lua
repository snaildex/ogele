linkTexture(Single2D, "BRDFLUT", {minFilter=LinearMipMapNearest, magFilter=Linear, wrap=ClampToEdge})

gbuffer = createRenderTarget(screen.width, screen.height, {"AlbedoRough", "PosDepth", "NormalMetal", "Emission"}, {format=RGBA32F, minFilter=LinearMipMapNearest, magFilter=Linear, depthBuffer=true})
final = createRenderTarget(screen.width, screen.height, {"FinalTex"}, {format=RGB8, depthBuffer=true})
setFinalRenderTarget(final, 0)

lightingComputeShader = findShader("LightingCompute")

function render ()
	clearRenderTarget(gbuffer, {renderTargetBuffer.color, renderTargetBuffer.depth}, {0.6,0.8,1.0,1.0})
	clearRenderTarget(final, {renderTargetBuffer.depth}, {0.6,0.8,1.0,1.0})

	enable(feature.depthTest)
	enable(feature.cullFace)
	disable(feature.blend)
	drawScene(gbuffer, {}, {"Opaque"})

	drawScreen(final, lightingComputeShader)

	disable(feature.depthTest)
	disable(feature.cullFace)
	enable(feature.blend)
	blendFunc(blendFactor.srcAlpha, blendFactor.one);
	drawScene(final, {}, {"Sparks"})
	blendFunc(blendFactor.srcAlpha, blendFactor.oneMinusSrcAlpha);
	drawScene(final, {}, {"Particles"})	
end

function resize ()
	resizeRenderTarget(gbuffer, screen.width, screen.height)
	resizeRenderTarget(final, screen.width, screen.height)
end
