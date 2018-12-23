#pragma once
namespace ogele {
	class RenderPipelineProxy : public ResourceProxy {
	public:
		struct RenderTargetDesciption {
			std::string Name;
			std::vector<std::string> Colors;
			TextureFormat Format;
			TextureFilterMode MinFilter;
			TextureFilterMode MagFilter;
			bool Mipmaps;
			bool Depth;
			bool Stencil;
		};
		struct PassDescription {
			std::string Name;
			RenderPipeline::PassMode Mode;
			std::vector<std::string> Tags;
			std::vector<std::string> Shader;
			std::vector<std::string> Mipmap;
			std::string Target;
			BufferBit Clear;
			ivec3 GroupNum;
		};
		struct TexDescription {
			std::string Name;
			TextureType Type;
			TextureFormat Format;
			TextureWrapMode Wrap;
			TextureFilterMode MinFilter;
			TextureFilterMode MagFilter;
			int Count;
			int Width;
			int Height;
			int Depth;
			std::vector<fs::path> Files;
		};
	private:
		std::vector<RenderTargetDesciption> m_targets;
		std::vector<PassDescription> m_passes;
		std::vector<TexDescription> m_textures;
	public:
		void AddTarget(const RenderTargetDesciption& target) noexcept { m_targets.push_back(target); }
		void AddPass(const PassDescription& pass) noexcept { m_passes.push_back(pass); }
		void AddTexture(const TexDescription& pass) noexcept { m_textures.push_back(pass); }
		virtual Resource* Build() const override;
	};

	class RenderPipelineLoader : public ResourceLoader {
	public:
		ResourceProxy *Load(const Jzon::Node* reader) const override;
	};
}