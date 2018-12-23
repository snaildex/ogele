#pragma once
namespace ogele {
    class Texture2DArray : public TextureBase {
	public:
		class Proxy;

	private:
        glm::ivec3 m_size;
		
        void Allocate(const glm::ivec3 &newSize);

    public:
        Texture2DArray(const glm::ivec3 &size, bool mipmaps, TextureFormat format);

		 const glm::ivec3 &GetSize() const noexcept { return m_size; }
		
        template<typename T>
        void bSetData(int level, const glm::ivec3 &offset, const glm::ivec3 &size, const T *data);

        template<typename T>
        void bGetData(int level, T *data) const;

    };


}
