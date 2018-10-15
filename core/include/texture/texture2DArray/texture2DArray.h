#pragma once

#include <glm/glm.hpp>

#include "../textureBase.h"
#include "../../other/enums.h"

namespace ogele {
    class Texture2DArray : public TextureBase {
        glm::ivec2 m_size;

        void Allocate(const glm::ivec2 &newSize);

    public:
        Texture2DArray(const glm::ivec2 &size, bool mipmaps, TextureFormat format);

        inline const glm::ivec2 &GetSize() const noexcept { return m_size; }

        template<typename T>
        void bSetData(int level, const glm::ivec2 &offset, const glm::ivec2 &size, const T *data);

        template<typename T>
        void bGetData(int level, T *data) const;

    };


}
