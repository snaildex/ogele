#pragma once
namespace ogele {

	class ResourceContainer;
	class ScreenQuadMesh;
	class CoordBasisMesh;
	class ShaderProgram;
	class TextureBase;
	class RenderPipeline;

	class Application : public Window {
		Entity(Application)

	private:
		static Application *m_instance;
		std::unique_ptr<ResourceContainer> m_res;
		glm::ivec2 m_screenSize;
		std::chrono::high_resolution_clock::time_point m_startTimePoint;
		std::chrono::high_resolution_clock::time_point m_lastFrameTimePoint;
		double m_timeDelta, m_time;
		std::unique_ptr<Material> m_material;
		nk_context *m_nuklear;
		nk_font_atlas *m_fontAtlas;
		std::mutex m_loadingQueueLock;
		std::vector<GLFWwindow*> m_loaderWindows;
		std::vector<std::thread> m_loaders;
		std::queue<std::packaged_task<Resource*(void)>> m_loading;
		static void LoaderFunc(GLFWwindow* window);

		void CreateInternal();

		dvec3 m_sceneOffset;

		ScreenQuadMesh *m_screenQuad;
		CoordBasisMesh *m_coordBasis;
		ShaderProgram *m_drawTex;
		ShaderProgram *m_drawBasis;

		std::unique_ptr<GPUStopwatch<100>> m_fpsCounter;
		std::list<std::unique_ptr<Transform>> m_roots;

	protected:
		virtual void Start() {};
		virtual void Update() {};
		virtual void Shutdown() {};

	public:
		Application(const std::string& caption);

		static Application *GetInstance() noexcept { return m_instance; }
		static double GetTime() noexcept { return m_instance->m_time; }
		static double GetTimeDelta() noexcept { return m_instance->m_timeDelta; }
		static const std::list<std::unique_ptr<Transform>>& GetRoots() noexcept { return m_instance->m_roots; }
		static Transform* CreateTransform() noexcept;
		static Actor* CreateActor(const std::string& name) noexcept;
		template<typename T>  static res_ptr<T> GetResourceByName(const std::string& name) noexcept { return m_instance->m_res->GetResourceByName<T>(name); }
		template<typename T>  static res_ptr<T> GetResourceByTag(const std::string& tag) noexcept { return m_instance->m_res->GetResourceByTag<T>(tag); }
		template<typename T>  static res_ptr<T> GetResourceByTags(const std::vector<std::string>& tags) noexcept { return m_instance->m_res->GetResourceByTags<T>(tags); }
		template<typename T>  static std::vector<res_ptr<T>> GetResources() noexcept { return m_instance->m_res->GetResources<T>(); }
		template<typename T>  static std::vector<res_ptr<T>> GetResourcesByTag(const std::string& tag) noexcept { return m_instance->m_res->GetResourcesByTag<T>(tag); }
		template<typename T>  static std::vector<res_ptr<T>> GetResourcesByTags(const std::vector<std::string>& tags) noexcept { return m_instance->m_res->GetResourcesByTags<T>(tags); }
		static const glm::ivec2 &GetScreenResolution() noexcept { return m_instance->m_screenSize; }
		static void DrawScreenQuad() { m_instance->m_screenQuad->Draw(); }
		static void DrawTex(TextureBase *tex);
		static void DrawBasis(const glm::dmat4 &MVP);
		static const dvec3& GetSceneOffset() noexcept { return m_instance->m_sceneOffset; }
		static dvec3 SceneToSpatial(const dvec3& sceneCoord) noexcept { return sceneCoord + m_instance->m_sceneOffset; }
		static dvec3 SpatialToScene(const dvec3& spatialCoord) noexcept { return spatialCoord - m_instance->m_sceneOffset; }
		static std::future<Resource*> LoadAsync(const ResourceProxy* proxy);
		static const Material* GetMaterial() noexcept { return m_instance->m_material.get(); }

		void Run();

		~Application();
	};

}
