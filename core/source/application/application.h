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
		std::mutex m_loadingQueueLock;
		std::vector<GLFWwindow*> m_loaderWindows;
		std::vector<std::thread> m_loaders;
		std::queue<std::packaged_task<Resource*(void)>> m_loading;
		fs::path m_startupPath;
		static void LoaderFunc(GLFWwindow* window);

		void CreateInternal();

		dvec3 m_sceneOffset;

		unique_ptr<ScreenQuadMesh> m_screenQuad;
		unique_ptr<CoordBasisMesh> m_coordBasis;
		unique_ptr<ShaderProgram> m_drawTex;
		unique_ptr<ShaderProgram> m_drawBasis;
		unique_ptr<ShaderProgram> m_drawLines;
		unique_ptr<DoubleBuffer<dvec3>> m_linesCoordsBuf;
		unique_ptr<FloatBuffer<vec3>> m_linesColorsBuf;
		unique_ptr<VertexArray> m_linesVao;
		int m_linesCount;
		int m_currentLinesCount;
		Camera* m_debugCamera;
		dvec3* m_linesCoordsData;
		vec3* m_linesColorsData;

		std::unique_ptr<GPUStopwatch<100>> m_fpsCounter;
		std::list<std::unique_ptr<World>> m_worlds;

		void InitLineDrawer();
		void PushLineVertex(const dvec3& coord, const vec3& color);
		void LineDrawerStartFrame();
		void LineDrawerEndFrame();
		void ShutdownLineDrawer();

	protected:
		virtual void Start() {};
		virtual void Update() {};
		virtual void Shutdown() {};

	public:
		Application(const std::string& caption);

		static Application *GetInstance() noexcept { return m_instance; }
		static double GetTime() noexcept { return m_instance->m_time; }
		static double GetTimeDelta() noexcept { return m_instance->m_timeDelta; }
		static const std::list<std::unique_ptr<World>>& GetWorlds() noexcept { return m_instance->m_worlds; }
		static World* CreateWorld(const std::string& name);
		template<typename T>  static res_ptr<T> GetResourceByName(const std::string& name) noexcept { return m_instance->m_res->GetResourceByName<T>(name); }
		template<typename T>  static res_ptr<T> GetResourceByTag(const std::string& tag) noexcept { return m_instance->m_res->GetResourceByTag<T>(tag); }
		template<typename T>  static res_ptr<T> GetResourceByTags(const std::vector<std::string>& tags) noexcept { return m_instance->m_res->GetResourceByTags<T>(tags); }
		template<typename T>  static std::vector<res_ptr<T>> GetResources() noexcept { return m_instance->m_res->GetResources<T>(); }
		template<typename T>  static std::vector<res_ptr<T>> GetResourcesByTag(const std::string& tag) noexcept { return m_instance->m_res->GetResourcesByTag<T>(tag); }
		template<typename T>  static std::vector<res_ptr<T>> GetResourcesByTags(const std::vector<std::string>& tags) noexcept { return m_instance->m_res->GetResourcesByTags<T>(tags); }
		static const glm::ivec2 &GetScreenResolution() noexcept { return m_instance->m_screenSize; }
		static void DrawScreenQuad() { m_instance->m_screenQuad->Draw(); }
		static dvec2 WorldToScreen(const dvec3& pos);
		static void DrawTex(TextureBase *tex);
		static void DrawTex(TextureBase *tex, const dvec2& size, const dvec2& pos, double rot = 0);
		static void DrawBasis(const glm::dmat4 &M);
		static const dvec3& GetSceneOffset() noexcept { return m_instance->m_sceneOffset; }
		static dvec3 SceneToSpatial(const dvec3& sceneCoord) noexcept { return sceneCoord + m_instance->m_sceneOffset; }
		static dvec3 SpatialToScene(const dvec3& spatialCoord) noexcept { return spatialCoord - m_instance->m_sceneOffset; }
		static std::future<Resource*> LoadAsync(const ResourceProxy* proxy);
		static const Material* GetMaterial() noexcept { return m_instance->m_material.get(); }
		static std::chrono::high_resolution_clock::time_point GetStartTime() noexcept { return m_instance->m_startTimePoint; }
		static const fs::path& GetStartupPath() noexcept { return m_instance->m_startupPath; }
		static fs::path RelativePath(const fs::path& path) noexcept { return path.lexically_relative(m_instance->m_startupPath); }
		static void SetDebugCamera(Camera* cam) { m_instance->m_debugCamera = cam; }
		static void DrawLine(const std::vector<glm::dvec3>& coords, const vec3& color);
		static void DrawRay(const glm::dvec3& pos, const glm::dvec3& dir, const vec3& color);

		void Run();

		~Application();
	};

}
