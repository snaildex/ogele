#pragma once
namespace ogele::GUI {
	namespace Internal {
		void Init(GLFWwindow* window);
		void Shutdown();
		void NewFrame();
		void Render();
	}

	enum class TextAlign {
		TopLeft = NK_TEXT_ALIGN_TOP | NK_TEXT_ALIGN_LEFT,
		TopCenter = NK_TEXT_ALIGN_TOP | NK_TEXT_ALIGN_CENTERED,
		TopRight = NK_TEXT_ALIGN_TOP | NK_TEXT_ALIGN_RIGHT,
		Left = NK_TEXT_ALIGN_MIDDLE | NK_TEXT_ALIGN_LEFT,
		Center = NK_TEXT_ALIGN_MIDDLE | NK_TEXT_ALIGN_CENTERED,
		Right = NK_TEXT_ALIGN_MIDDLE | NK_TEXT_ALIGN_RIGHT,
		BottomLeft = NK_TEXT_ALIGN_BOTTOM | NK_TEXT_ALIGN_LEFT,
		BottomCenter = NK_TEXT_ALIGN_BOTTOM | NK_TEXT_ALIGN_CENTERED,
		BottomRight = NK_TEXT_ALIGN_BOTTOM | NK_TEXT_ALIGN_RIGHT
	};
	EnumFlags(TextAlign);
	enum class WindowFlags {
		None = 0,
		Private = NK_WINDOW_PRIVATE,
		//Special window type growing up in height while being filled to a certain maximum height
		Dynamic = NK_WINDOW_DYNAMIC,
		//Sets window widgets into a read only mode and does not allow input changes
		ROM = NK_WINDOW_ROM,
		//Prevents all interaction caused by input to either window or widgets inside
		NotInteractive = NK_WINDOW_NOT_INTERACTIVE,
		//Hides window and stops any window interaction and drawing
		Hidden = NK_WINDOW_HIDDEN,
		//Directly closes and frees the window at the end of the frame
		Closed = NK_WINDOW_CLOSED,
		//marks the window as minimized */
		Minimized = NK_WINDOW_MINIMIZED,
		//Removes read only mode at the end of the window
		RemoveROM = NK_WINDOW_REMOVE_ROM,
		//Draws a border around the window to visually separate window from the background
		Border = NK_WINDOW_BORDER,
		//The movable flag indicates that a window can be moved by user input or by dragging the window header
		Movable = NK_WINDOW_MOVABLE,
		//The scalable flag indicates that a window can be scaled by user input by dragging a scaler icon at the button of the window
		Scalable = NK_WINDOW_SCALABLE,
		//Adds a closable icon into the header
		Closable = NK_WINDOW_CLOSABLE,
		//Adds a minimize icon into the header
		Minimizable = NK_WINDOW_MINIMIZABLE,
		//Removes the scrollbar from the window
		NoScrollbar = NK_WINDOW_NO_SCROLLBAR,
		//Forces a header at the top at the window showing the title
		Title = NK_WINDOW_TITLE,
		//Automatically hides the window scrollbar if no user interaction
		ScrollAutoHide = NK_WINDOW_SCROLL_AUTO_HIDE,
		//Always keep window in the background
		Background = NK_WINDOW_BACKGROUND,
		//Puts window scaler in the left-bottom corner instead right-bottom
		ScaleLeft = NK_WINDOW_SCALE_LEFT,
		//Prevents window of scaling, moving or getting focus
		NoInput = NK_WINDOW_NO_INPUT
	};
	EnumFlags(WindowFlags);
	enum class TreeType {
		Node = NK_TREE_NODE,
		Tab = NK_TREE_TAB
	};
	enum class CollapseState {
		Maximized = NK_MAXIMIZED,
		Minimized = NK_MINIMIZED
	};

	namespace Layout {
		void SetMinRowHeight(float height);
		void ResetMinRowHeight();
		void RowDynamic(int columns, float height = 0.0f);
		void RowDynamic(int columns, std::function<void(void)> action, float height = 0.0f);
		void RowStatic(int columns, int columnWidth, float height = 0.0f);
		void RowStatic(int columns, std::function<void(void)> action, float height = 0.0f);
		void RowPush(float value);
		void RowTemplate(std::function<void(void)> action, float height = 0.0f);
		void TemplateDynamic();
		void TemplateVariable(float minWidth);
		void TemplateStatic(float width);
		void SpaceDynamic(int widgets, std::function<void(void)> action, float height = 0.0f);
		void SpaceStatic(int widgets, std::function<void(void)> action, float height = 0.0f);
		void SpacePush(const vec2& position, const vec2& size);
	}

	void Window(const std::string& caption, WindowFlags flags, const vec2& position, const vec2& size, std::function<void(void)> action);
	void Group(const std::string& title, WindowFlags flags, std::function<void(void)> action);
	void Tree(const std::string& title, const void* hash, int hasSize, int seed, std::function<void(void)> action, TreeType type = TreeType::Node, CollapseState state = CollapseState::Minimized);
	template<typename T>
	void Tree(const std::string& title, const T* hash, int seed, std::function<void(void)> action, TreeType type = TreeType::Node, CollapseState state = CollapseState::Minimized) {
		Tree(title, hash, sizeof(T), seed, action, type, state);
	}
	void Label(const std::string& text, TextAlign align = TextAlign::Left);
	void LabelColored(const std::string& text, const glm::vec4& color, TextAlign align = TextAlign::Left);
	void Image(const Texture2D* image);
}