#include <stdafx.h>
#include <ogele.h>
using namespace std;
namespace ogele {
	const map<const string, BufferBit> StrToBufBit = {
		{"Color",BufferBit::Color},
		{"Depth",BufferBit::Depth},
		{"Stencil",BufferBit::Stencil},
		{"Accumulation",BufferBit::Accumulation},
	};
	const array<string, 6> FacesNames = {
			"right",
			"left",
			"up",
			"down",
			"back",
			"front"
	};
	Resource* RenderPipelineProxy::Build() const {
		map<string, RenderTarget*> targets;
		RenderPipeline *res = new RenderPipeline();
		res->CopyNameTagsFrom(this);
		for (const auto& t : m_targets) {
			RenderTarget* rt = new RenderTarget(
				Application::GetInstance()->GetResolution(),
				t.Colors.size(),
				t.Format,
				t.MinFilter,
				t.MagFilter,
				t.Mipmaps,
				t.Depth,
				t.Stencil
			);
			for (int i = 0; i < t.Colors.size(); i++)
				res->GetMaterial()->SetTexture(t.Colors[i], (*rt)[i]);
			res->AddRenderTarget(t.Name, t.Size, rt);
			targets[t.Name] = rt;
		}
		for (const auto& t : m_textures) {
			bool mipmaps = t.MinFilter == TextureFilterMode::LinearMipMapLinear ||
				t.MinFilter == TextureFilterMode::LinearMipMapNearest ||
				t.MinFilter == TextureFilterMode::NearestMipMapLinear ||
				t.MinFilter == TextureFilterMode::NearestMipMapNearest;
			if (t.Type == TextureType::Single2D) {
				int w = t.Width, h = t.Height;
				ucvec4* data = nullptr;
				if (t.Files.size() == 1)
					data = reinterpret_cast<ucvec4 *>(stbi_load(t.Files[0].string().c_str(), &w, &h, nullptr, 4));
				Texture2D* tex = new Texture2D({ t.Width,t.Height }, mipmaps, t.Format);
				tex->SetName(t.Name);
				tex->Bind();
				tex->bSetWrap(t.Wrap);
				tex->bSetMinFilter(t.MinFilter);
				tex->bSetMagFilter(t.MagFilter);
				if (data != nullptr)
					tex->bSetData(0, { 0, 0 }, { w, h }, data);
				tex->Unbind();
				res->AddTexture(tex);
				if (t.Files.size() == 1)
					stbi_image_free(data);
			}
			if (t.Type == TextureType::Single3D) {
				int w = t.Width, h = t.Height;
				Texture3D* tex = new Texture3D({ t.Width,t.Height,t.Depth }, mipmaps, t.Format);
				tex->SetName(t.Name);
				tex->Bind();
				tex->bSetWrap(t.Wrap);
				tex->bSetMinFilter(t.MinFilter);
				tex->bSetMagFilter(t.MagFilter);
				tex->Unbind();
				res->AddTexture(tex);
			}
			if (t.Type == TextureType::SingleCubeMap) {
				int w = t.Width, h = t.Height;
				ucvec4** data = new ucvec4*[6];
				for (size_t i = 0; i < 6; ++i)
					data[i] = nullptr;
				if (t.Files.size() == 6)
					for (size_t i = 0; i < 6; ++i)
						data[i] = reinterpret_cast<ucvec4 *>(stbi_load(t.Files[0].string().c_str(), &w, &h, nullptr, 4));
				TextureCube* tex = new TextureCube({ w,h }, mipmaps, t.Format);
				tex->SetName(t.Name);
				tex->Bind();
				tex->bSetWrap(t.Wrap);
				tex->bSetMinFilter(t.MinFilter);
				tex->bSetMagFilter(t.MagFilter);
				for (size_t i = 0; i < 6; ++i)
					if (data[i] != nullptr)
						tex->bSetData<ucvec4>(i, 0, ivec2(0, 0), ivec2(t.Width, t.Height), data[i]);
				tex->Unbind();
				res->AddTexture(tex);
				if (t.Files.size() == 6)
					for (size_t i = 0; i < 6; ++i)
						stbi_image_free(data[i]);
			}
		}
		for (const auto& p : m_passes) {
			vector<TextureBase*> mipmaps;
			for (const auto& m : p.Mipmap)
				mipmaps.push_back(res->GetTexture(m));
			res->AddPass({
				p.Name,
				p.Mode,
				p.Tags,
				p.Shader,
				mipmaps,
				targets.at(p.Target),
				p.Mode != RenderPipeline::PassMode::Scene ? Application::GetResourceByName<ShaderProgram>(p.Shader[0]) : nullptr,
				p.Clear,
				p.GroupNum
				});
		}
		for (const auto& p : m_inits) {
			vector<TextureBase*> mipmaps;
			for (const auto& m : p.Mipmap)
				mipmaps.push_back(res->GetTexture(m));
			res->AddInit({
				p.Name,
				p.Mode,
				p.Tags,
				p.Shader,
				mipmaps,
				targets.at(p.Target),
				p.Mode != RenderPipeline::PassMode::Scene ? Application::GetResourceByName<ShaderProgram>(p.Shader[0]) : nullptr,
				p.Clear,
				p.GroupNum
				});
		}
		res->Init();
		return res;
	}

	ResourceProxy *RenderPipelineLoader::Load(const Jzon::Node *reader) const {
		RenderPipelineProxy *res = new RenderPipelineProxy();
		LoadNameTags(reader, res);
		Jzon::Node targets = reader->get("targets");
		for (size_t i = 0; i < targets.getCount(); ++i) {
			Jzon::Node target = targets.get(i);
			string name = ReadStringProperty(&target, "name", "Final");
			int colorNum = target.has("colorTex") ? target.get("colorTex").getCount() : 1;
			vector<string> colors;
			if (target.has("colorTex"))
				for (int i = 0; i < colorNum; ++i)
					colors.push_back(target.get("colorTex").get(i).toString());
			else
				colors.push_back("FinalTex");
			res->AddTarget({
					ReadStringProperty(&target, "name", "Final"),
					colors,
					StrToTexFormat.at(ReadStringProperty(&target, "format", "RGB8")),
					StrToTexFilterMode.at(ReadStringProperty(&target, "minFilter", "nearest")),
					StrToTexFilterMode.at(ReadStringProperty(&target, "magFilter", "nearest")),
					ReadBoolProperty(&target, "mipmaps", false),
					ReadBoolProperty(&target, "depthBuffer", false),
					ReadBoolProperty(&target, "stencilBuffer", false),
					vec2(ReadFloatProperty(&target,"width",1.0f),ReadFloatProperty(&target,"height",1.0f))
				});
		}
		Jzon::Node passes = reader->get("pass");
		for (size_t i = 0; i < passes.getCount(); ++i) {
			Jzon::Node pass = passes.get(i);
			BufferBit clear = (BufferBit)0;
			if (pass.has("clear"))
				for (size_t i = 0; i < pass.get("clear").getCount(); ++i)
					clear = clear | StrToBufBit.at(pass.get("clear").get(i).toString());
			ivec3 groupNum = { 1,1,1 };
			if (pass.has("groupNum")) {
				Jzon::Node gnum = pass.get("groupNum");
				if (gnum.isArray()) {
					size_t count = std::min(size_t(3), gnum.getCount());
					for (size_t i = 0; i < count; ++i)
						groupNum[i] = gnum.get(i).toInt();
				}
				else
					groupNum[0] = gnum.toInt();
			}
			res->AddPass({
				ReadStringProperty(&pass,"name","Unnamed"),
				RenderPipeline::StrToMode[ReadStringProperty(&pass,"mode","scene")],
				ReadStringArrayProperty(&pass,"tags"),
				ReadStringArrayProperty(&pass,"shader"),
				ReadStringArrayProperty(&pass,"mipmap"),
				ReadStringProperty(&pass,"target","Final"),
				clear,
				groupNum
				});
		}
		Jzon::Node inits = reader->get("init");
		for (size_t i = 0; i < inits.getCount(); ++i) {
			Jzon::Node pass = inits.get(i);
			BufferBit clear = (BufferBit)0;
			if (pass.has("clear"))
				for (size_t i = 0; i < pass.get("clear").getCount(); ++i)
					clear = clear | StrToBufBit.at(pass.get("clear").get(i).toString());
			ivec3 groupNum = { 1,1,1 };
			if (pass.has("groupNum")) {
				Jzon::Node gnum = pass.get("groupNum");
				if (gnum.isArray()) {
					size_t count = std::min(size_t(3), gnum.getCount());
					for (size_t i = 0; i < count; ++i)
						groupNum[i] = gnum.get(i).toInt();
				}
				else
					groupNum[0] = gnum.toInt();
			}
			res->AddPass({
				ReadStringProperty(&pass,"name","Unnamed"),
				RenderPipeline::StrToMode[ReadStringProperty(&pass,"mode","scene")],
				ReadStringArrayProperty(&pass,"tags"),
				ReadStringArrayProperty(&pass,"shader"),
				ReadStringArrayProperty(&pass,"mipmap"),
				ReadStringProperty(&pass,"target","Final"),
				clear,
				groupNum
				});
		}
		if (reader->has("textures")) {
			Jzon::Node textures = reader->get("textures");
			for (size_t i = 0; i < textures.getCount(); ++i) {
				Jzon::Node tex = textures.get(i);
				vector<fs::path> files;
				if (tex.has("file")) {
					Jzon::Node file = tex.get("file");
					if (file.isArray())
						for (size_t i = 0; i < file.getCount(); ++i)
							files.push_back(fs::absolute(file.get(i).toString()));
					if (file.isObject())
						for (size_t i = 0; i < 6; ++i)
							files.push_back(fs::absolute(file.get(FacesNames[i]).toString()));
					if (file.isString())
						files.push_back(fs::absolute(file.toString()));
				}
				res->AddTexture({
					ReadStringProperty(&tex, "name", "Unnamed"),
					StrToTexType.at(ReadStringProperty(&tex, "type", "Single2D")),
					StrToTexFormat.at(ReadStringProperty(&tex, "format", "RGB8")),
					StrToTexWrapMode.at(ReadStringProperty(&tex, "wrap", "clampToEdge")),
					StrToTexFilterMode.at(ReadStringProperty(&tex, "minFilter", "nearest")),
					StrToTexFilterMode.at(ReadStringProperty(&tex, "magFilter", "nearest")),
					ReadIntProperty(&tex,"count", 0),
					ReadIntProperty(&tex,"width", 512),
					ReadIntProperty(&tex,"height", 512),
					ReadIntProperty(&tex,"depth", 512),
					files
					});
			}
		}
		return res;
	}
}
