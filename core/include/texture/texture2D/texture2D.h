//
// Created by ??????? on 21.07.2018.
//

#ifndef OGELE_TEXTURE2D_H
#define OGELE_TEXTURE2D_H
namespace ogele {
    class Texture2D : public TextureBase {
        ivec2 m_size;

        void Allocate(const ivec2 &newSize);

    public:
        Texture2D(const ivec2 &size, bool mipmaps, TextureFormat format);

        inline const ivec2 &GetSize() const noexcept { return m_size; }

        template<typename T>
        void bSetData(int level, const ivec2 &offset, const ivec2 &size, const T *data);

        template<typename T>
        void bGetData(int level, T *data) const;

    };


}
#endif //OGELE_TEXTURE2D_H
