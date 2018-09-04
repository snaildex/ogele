//
// Created by ??????? on 25.08.2018.
//

#ifndef OGELE_TEXTURE3D_H
#define OGELE_TEXTURE3D_H

namespace ogele {
    class Texture3D : public TextureBase {
        ivec3 m_size;

        void Allocate(const ivec3 &newSize);

    public:
        Texture3D(const ivec3 &size, bool mipmaps, TextureFormat format);

        inline const ivec3 &GetSize() const noexcept { return m_size; }

        template<typename T>
        void bSetData(int level, const ivec3 &offset, const ivec3 &size, const T *data);

        template<typename T>
        void bGetData(int level, T *data) const;

    };


}

#endif //OGELE_TEXTURE3D_H
