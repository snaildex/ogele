#pragma once
namespace ogele {
	class Texture2D : public TextureBase {
	public:
		class Proxy;

	private:
		glm::ivec2 m_size;

		void Allocate(const glm::ivec2 &newSize);

	public:
		Texture2D(const glm::ivec2 &size, bool mipmaps, TextureFormat format);

		const glm::ivec2 &GetSize() const noexcept { return m_size; }

		void Resize(const glm::ivec2& newSize);

		template<typename T>
		void bSetData(int level, const glm::ivec2 &offset, const glm::ivec2 &size, const T *data);

		template<typename T>
		void bGetData(int level, T *data) const;

	};
}