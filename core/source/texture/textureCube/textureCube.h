#pragma once
namespace ogele {
	class TextureCube : public TextureBase {
	public:
		class Proxy;

	private:
        glm::ivec2 m_size;

        void Allocate(const glm::ivec2 &newSize);

    public:
        TextureCube(const glm::ivec2 &size, bool mipmaps, TextureFormat format);

         const glm::ivec2 &GetSize() const noexcept { return m_size; }

        template<typename T>
        void bSetData(int face, int level, const glm::ivec2 &offset, const glm::ivec2 &size, const T *data);

        template<typename T>
        void bGetData(int face, int level, T *data) const;

    };


}