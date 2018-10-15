#pragma once

#include <glm/glm.hpp>

#include "../other/enums.h"
#include "../other/glerror.h"
#include "../object/resource.h"

namespace ogele {
    class TextureBase : public HandleBase<GLenum>, public Resource {
        TextureType m_type;
        TextureFormat m_format;
        bool m_mipmaps;
        int m_bindLevel;
        bool m_bindLayered;
        int m_bindLayer;
        AccessMode m_bindAccess;

    protected:
        TextureBase(TextureType type, TextureFormat format, bool mipmaps);

    public:
        inline void Bind() const { glBindTexture(static_cast<GLenum>(m_type), m_handle); GLErr(); }

        inline void Unbind() const { glBindTexture(static_cast<GLenum>(m_type), 0); GLErr(); }

        inline TextureType GetType() const noexcept { return m_type; }

        inline TextureFormat GetFormat() const noexcept { return m_format; }

        bool IsInteger() const noexcept;

        inline bool HasMipmaps() const noexcept { return m_mipmaps; }

        inline int GetBindLevel() const noexcept { return m_bindLevel; }

        inline bool IsBindLayered() const noexcept { return m_bindLayered; }

        inline int GetBindLayer() const noexcept { return m_bindLayer; }

        inline AccessMode GetBindAccessMode() const noexcept { return m_bindAccess; }

        inline void SetBindLevel(int value) noexcept { m_bindLevel = value; }

        inline void SetBindLayered(bool value) noexcept { m_bindLayered = value; }

        inline void SetBindLayer(int value) noexcept { m_bindLayer = value; }

        inline void SetBindAccessMode(AccessMode value) noexcept { m_bindAccess = value; }

        void BindImage(int unitNum) const;

        TextureDepthStencilMode bGetDepthStencilMode() const;

        void bSetDepthStencilMode(TextureDepthStencilMode value);

        glm::vec4 bGetBorderColor() const;

        void bSetBorderColor(glm::vec4 value);

        DepthFuncMode bGetCompareFunc() const;

        void bSetCompareFunc(DepthFuncMode value);

        TextureCompareMode bGetCompareMode() const;

        void bSetCompareMode(TextureCompareMode value);

        TextureFilterMode bGetMinFilter() const;

        void bSetMinFilter(TextureFilterMode value);

        TextureFilterMode bGetMagFilter() const;

        void bSetMagFilter(TextureFilterMode value);

        int bGetBaseLevel() const;

        void bSetBaseLevel(int value);

        int bGetMaxLevel() const;

        void bSetMaxLevel(int value);

        float bGetLODBias() const;

        void bSetLODBias(float value);

        float bGetMinLOD() const;

        void bSetMinLOD(float value);

        float bGetMaxLOD() const;

        void bSetMaxLOD(float value);

        ChannelSwizzleValue bGetSwizzleR() const;

        void bSetSwizzleR(ChannelSwizzleValue value);

        ChannelSwizzleValue bGetSwizzleG() const;

        void bSetSwizzleG(ChannelSwizzleValue value);

        ChannelSwizzleValue bGetSwizzleB() const;

        void bSetSwizzleB(ChannelSwizzleValue value);

        ChannelSwizzleValue bGetSwizzleA() const;

        void bSetSwizzleA(ChannelSwizzleValue value);

        TextureWrapMode bGetWrapS() const;

        void bSetWrapS(TextureWrapMode value);

        TextureWrapMode bGetWrapT() const;

        void bSetWrapT(TextureWrapMode value);

        TextureWrapMode bGetWrapR() const;

        void bSetWrapR(TextureWrapMode value);

        void bSetWrap(TextureWrapMode value);

        void bGenerateMipMap();

        void SetActive(int num);

        ~TextureBase();
    };
}