#pragma once
#include <memory>
#include <chrono>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "window.h"
#include "../object/resource.h"
#include "../texture/textureBase.h"
#include "../shader/shaderProgram.h"
#include "../mesh/screenQuadMesh.h"
#include "../mesh/coordBasisMesh.h"
#include "../helpers/helpers.h"

namespace ogele {

    class ResourceContainer;

    class ScreenQuadMesh;

    class CoordBasisMesh;

    class ShaderProgram;

    class TextureBase;

    class Application : public Window {
        Entity(Application)

    private:
        static Application *m_instance;
        std::unique_ptr<ResourceContainer> m_res;
        glm::ivec2 m_screenSize;
        std::chrono::high_resolution_clock::time_point m_startTimePoint;
        std::chrono::high_resolution_clock::time_point m_lastFrameTimePoint;
        double m_timeDelta, m_time;

        void CreateInternal();

        ScreenQuadMesh *m_screenQuad;
        CoordBasisMesh *m_coordBasis;
        ShaderProgram *m_drawTex;
        ShaderProgram *m_drawBasis;
    protected:
        virtual void Start() {};

        virtual void Update() {};

        virtual void Shutdown() {};

        void DrawTex(TextureBase *tex);

        void DrawBasis(const glm::dmat4 &MVP);

    public:
        Application();

        inline ScreenQuadMesh *GetScreenQuadMesh() const noexcept { return m_screenQuad; }

        inline static Application *GetInstance() noexcept { return m_instance; }

        inline double GetTime() const noexcept { return m_time; }

        inline double GetTimeDelta() const noexcept { return m_timeDelta; }

        inline ResourceContainer *GetResources() noexcept { return m_res.get(); }

        inline const glm::ivec2 &GetScreenResolution() const noexcept { return m_screenSize; }

        void Run();

        ~Application();
    };

}
