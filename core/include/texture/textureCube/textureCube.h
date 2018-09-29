//
// Created by ??????? on 21.07.2018.
//

#ifndef OGELE_TEXTURECUBE_H
#define OGELE_TEXTURECUBE_H
namespace ogele {
    class TextureCube : public TextureBase {
        ivec2 m_size;

        void Allocate(const ivec2 &newSize);

    public:
        TextureCube(const ivec2 &size, bool mipmaps, TextureFormat format);

        inline const ivec2 &GetSize() const noexcept { return m_size; }

        template<typename T>
        void bSetData(int face, int level, const ivec2 &offset, const ivec2 &size, const T *data);

        template<typename T>
        void bGetData(int face, int level, T *data) const;

    };


}
#endif //OGELE_TEXTURECUBE_H
