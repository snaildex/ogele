#pragma once
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
         void Bind() const { glBindTexture(static_cast<GLenum>(m_type), m_handle); GLErr(); }

         void Unbind() const { glBindTexture(static_cast<GLenum>(m_type), 0); GLErr(); }

         TextureType GetType() const noexcept { return m_type; }

         TextureFormat GetFormat() const noexcept { return m_format; }

        bool IsInteger() const noexcept;

         bool HasMipmaps() const noexcept { return m_mipmaps; }

         int GetBindLevel() const noexcept { return m_bindLevel; }

         bool IsBindLayered() const noexcept { return m_bindLayered; }

         int GetBindLayer() const noexcept { return m_bindLayer; }

         AccessMode GetBindAccessMode() const noexcept { return m_bindAccess; }

         void SetBindLevel(int value) noexcept { m_bindLevel = value; }

         void SetBindLayered(bool value) noexcept { m_bindLayered = value; }

         void SetBindLayer(int value) noexcept { m_bindLayer = value; }

         void SetBindAccessMode(AccessMode value) noexcept { m_bindAccess = value; }

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

        virtual ~TextureBase();
    };
}