#pragma once
namespace ogele {
	class RenderPipelineProxy;

	class RenderPipeline : public Resource {
	public:
		typedef RenderPipelineProxy Proxy;

		enum class PassMode : GLenum {
			Scene,
			Screen,
			Compute
		};

		struct Pass {
			std::string Name;
			PassMode Mode;
			std::vector<std::string> Tags;
			std::vector<std::string> Shader;
			std::vector<TextureBase*> Mipmap;
			RenderTarget* Target;
			res_ptr<ShaderProgram> SSShader;
			BufferBit Clear;
			ivec3 GroupNum;
		};

	private:
		std::unique_ptr<Material> m_material;
		std::map<std::string, unique_ptr<RenderTarget>> m_renderTargets;
		std::vector<Pass> m_passes;
		std::vector<std::unique_ptr<TextureBase>> m_textures;

	public:
		static std::map<std::string, PassMode> StrToMode;
		RenderPipeline();
		Material* GetMaterial() const { return m_material.get(); }
		RenderTarget* GetRenderTarget(const std::string& name) const { return m_renderTargets.at(name).get(); }
		TextureBase* GetTexture(const std::string& name) const { 
			return std::find_if(m_textures.cbegin(), m_textures.cend(), [&](const std::unique_ptr<TextureBase>& tex) { return tex->GetName() == name; })->get();
		}
		void AddRenderTarget(const std::string& name, RenderTarget* renderTarget) { m_renderTargets.emplace(name, std::unique_ptr<RenderTarget>(renderTarget)); }
		void AddPass(const Pass& pass) { m_passes.push_back(pass); }
		void AddTexture(TextureBase* texture) { m_textures.emplace_back(texture); m_material->SetTexture(texture->GetName(), texture); }
		void Render(Camera* cam) const;
	};

}