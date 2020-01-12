#pragma once
namespace ogele {
	class RenderPipelineProxy;

	class RenderPipeline : public Resource {
	public:
		typedef RenderPipelineProxy Proxy;

	private:
		std::unique_ptr<Lua::State> m_lua;
		std::vector<std::unique_ptr<TextureBase>> m_textures;
		std::vector<std::unique_ptr<RenderTarget>> m_renderTargets;
		std::unique_ptr<Material> m_material;
		Camera* m_cam;
		World* m_world;
		RenderTarget* m_finalTarget;
		int m_finalTargetTextureIndex;

		int CreateTexture(Lua::State* state);
		int LinkTexture(Lua::State* state);
		int CreateRenderTarget(Lua::State* state);
		int FindShader(Lua::State* state);
		int Enable(Lua::State* state);
		int Disable(Lua::State* state);
		int BlendFunc(Lua::State* state);
		int DispatchCompute(Lua::State* state);
		int DrawScreen(Lua::State* state);
		int DrawScene(Lua::State* state);
		int SetFinalRenderTarget(Lua::State* state);
		int ClearRenderTarget(Lua::State* state);
		int ResizeRenderTarget(Lua::State* state);

		void DrawRecursive(const Camera* cam, Transform* node, const std::vector<std::string>& actorTags, const std::vector<std::string>& shaderTags);

	public:
		RenderPipeline(const fs::path& sourcePath);
		~RenderPipeline();

		Material* GetMaterial() noexcept { return m_material.get(); }
		void SetCamera(Camera* cam) noexcept { m_cam = cam; }
		void SetWorld(World* world) noexcept { m_world = world; }

		void Render();
		void Resize(const ivec2& size);
	};

}