#pragma once
namespace ogele::GUI {

#define TextID(name) ( name +std::string("##")+std::to_string(reinterpret_cast<size_t>(this)))

	namespace Internal {
		void Init(GLFWwindow* window);
		void Shutdown();
		void NewFrame();
		void Render();
		template<typename T> void Property(const std::string& name, T& value);
	}

	namespace Place {
		void Text(vec2 screenPos, const char* format, ...);
	}

	enum class WindowFlags {
		None = 0,
		NoTitleBar = 1,   // Disable title-bar
		NoResize = 1 << 1,   // Disable user resizing with the lower-right grip
		NoMove = 1 << 2,   // Disable user moving the window
		NoScrollbar = 1 << 3,   // Disable scrollbars (window can still scroll with mouse or programmatically)
		NoScrollWithMouse = 1 << 4,   // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
		NoCollapse = 1 << 5,   // Disable user collapsing window by double-clicking on it
		AlwaysAutoResize = 1 << 6,   // Resize every window to its content every frame
		NoBackground = 1 << 7,   // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
		NoSavedSettings = 1 << 8,   // Never load/save settings in .ini file
		NoMouseInputs = 1 << 9,   // Disable catching mouse, hovering test with pass through.
		MenuBar = 1 << 10,  // Has a menu-bar
		HorizontalScrollbar = 1 << 11,  // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
		NoFocusOnAppearing = 1 << 12,  // Disable taking focus when transitioning from hidden to visible state
		NoBringToFrontOnFocus = 1 << 13,  // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
		AlwaysVerticalScrollbar = 1 << 14,  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
		AlwaysHorizontalScrollbar = 1 << 15,  // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
		AlwaysUseWindowPadding = 1 << 16,  // Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)
		NoNavInputs = 1 << 18,  // No gamepad/keyboard navigation within the window
		NoNavFocus = 1 << 19,  // No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)
		UnsavedDocument = 1 << 20,  // Append '*' to title without affecting the ID, as a convenience to avoid using the ### operator. When used in a tab/docking context, tab is selected on closure and closure is deferred by one frame to allow code to cancel the closure (with a confirmation popup, etc.) without flicker.
		NoNav = NoNavInputs | NoNavFocus,
		NoDecoration = NoTitleBar | NoResize | NoScrollbar | NoCollapse,
		NoInputs = NoMouseInputs | NoNavInputs | NoNavFocus,
	};
	EnumFlags(WindowFlags);
	enum class Direction {
		None = -1,
		Left = 0,
		Right = 1,
		Up = 2,
		Down = 3,
	};
	enum class ComboFlags {
		None = 0,
		PopupAlignLeft = 1 << 0,   // Align the popup toward the left by default
		HeightSmall = 1 << 1,   // Max ~4 items visible. Tip: If you want your combo popup to be a specific size you can use SetNextWindowSizeConstraints() prior to calling BeginCombo()
		HeightRegular = 1 << 2,   // Max ~8 items visible (default)
		HeightLarge = 1 << 3,   // Max ~20 items visible
		HeightLargest = 1 << 4,   // As many fitting items as possible
		NoArrowButton = 1 << 5,   // Display on the preview box without the square arrow button
		NoPreview = 1 << 6,   // Display only a square arrow button
		HeightMask = HeightSmall | HeightRegular | HeightLarge | HeightLargest
	};
	EnumFlags(ComboFlags);
	enum class InputTextFlags {
		None = 0,
		CharsDecimal = 1 << 0,   // Allow 0123456789.+-*/
		CharsHexadecimal = 1 << 1,   // Allow 0123456789ABCDEFabcdef
		CharsUppercase = 1 << 2,   // Turn a..z into A..Z
		CharsNoBlank = 1 << 3,   // Filter out spaces, tabs
		AutoSelectAll = 1 << 4,   // Select entire text when first taking mouse focus
		EnterReturnsTrue = 1 << 5,   // Return 'true' when Enter is pressed (as opposed to every time the value was modified). Consider looking at the IsItemDeactivatedAfterEdit() function.
		CallbackCompletion = 1 << 6,   // Callback on pressing TAB (for completion handling)
		CallbackHistory = 1 << 7,   // Callback on pressing Up/Down arrows (for history handling)
		CallbackAlways = 1 << 8,   // Callback on each iteration. User code may query cursor position, modify text buffer.
		CallbackCharFilter = 1 << 9,   // Callback on character inputs to replace or discard them. Modify 'EventChar' to replace or discard, or return 1 in callback to discard.
		AllowTabInput = 1 << 10,  // Pressing TAB input a '\t' character into the text field
		CtrlEnterForNewLine = 1 << 11,  // In multi-line mode, unfocus with Enter, add new line with Ctrl+Enter (default is opposite: unfocus with Ctrl+Enter, add line with Enter).
		NoHorizontalScroll = 1 << 12,  // Disable following the cursor horizontally
		AlwaysInsertMode = 1 << 13,  // Insert mode
		ReadOnly = 1 << 14,  // Read-only mode
		Password = 1 << 15,  // Password mode, display all characters as '*'
		NoUndoRedo = 1 << 16,  // Disable undo/redo. Note that input text owns the text data while active, if you want to provide your own undo/redo stack you need e.g. to call ClearActiveID().
		CharsScientific = 1 << 17,  // Allow 0123456789.+-*/eE (Scientific notation input)
		CallbackResize = 1 << 18,  // Callback on buffer capacity changes request (beyond 'buf_size' parameter value), allowing the string to grow. Notify when the string wants to be resized (for string types which hold a cache of their Size). You will be provided a new BufSize in the callback and NEED to honor it. (see misc/cpp/imgui_stdlib.h for an example of using this)

	};
	EnumFlags(InputTextFlags);
	enum class ColorEditFlags {
		None = 0,
		NoAlpha = 1 << 1,   //              // ColorEdit, ColorPicker, ColorButton: ignore Alpha component (will only read 3 components from the input pointer).
		NoPicker = 1 << 2,   //              // ColorEdit: disable picker when clicking on colored square.
		NoOptions = 1 << 3,   //              // ColorEdit: disable toggling options menu when right-clicking on inputs/small preview.
		NoSmallPreview = 1 << 4,   //              // ColorEdit, ColorPicker: disable colored square preview next to the inputs. (e.g. to show only the inputs)
		NoInputs = 1 << 5,   //              // ColorEdit, ColorPicker: disable inputs sliders/text widgets (e.g. to show only the small preview colored square).
		NoTooltip = 1 << 6,   //              // ColorEdit, ColorPicker, ColorButton: disable tooltip when hovering the preview.
		NoLabel = 1 << 7,   //              // ColorEdit, ColorPicker: disable display of inline text label (the label is still forwarded to the tooltip and picker).
		NoSidePreview = 1 << 8,   //              // ColorPicker: disable bigger color preview on right side of the picker, use small colored square preview instead.
		NoDragDrop = 1 << 9,   //              // ColorEdit: disable drag and drop target. ColorButton: disable drag and drop source.
		AlphaBar = 1 << 16,  //              // ColorEdit, ColorPicker: show vertical alpha bar/gradient in picker.
		AlphaPreview = 1 << 17,  //              // ColorEdit, ColorPicker, ColorButton: display preview as a transparent color over a checkerboard, instead of opaque.
		AlphaPreviewHalf = 1 << 18,  //              // ColorEdit, ColorPicker, ColorButton: display half opaque / half checkerboard, instead of opaque.
		HDR = 1 << 19,  //              // (WIP) ColorEdit: Currently only disable 0.0f..1.0f limits in RGBA edition (note: you probably want to use Float flag as well).
		DisplayRGB = 1 << 20,  // [Display]    // ColorEdit: override _display_ type among RGB/HSV/Hex. ColorPicker: select any combination using one or more of RGB/HSV/Hex.
		DisplayHSV = 1 << 21,  // [Display]    // "
		DisplayHex = 1 << 22,  // [Display]    // "
		Uint8 = 1 << 23,  // [DataType]   // ColorEdit, ColorPicker, ColorButton: _display_ values formatted as 0..255.
		Float = 1 << 24,  // [DataType]   // ColorEdit, ColorPicker, ColorButton: _display_ values formatted as 0.0f..1.0f floats instead of 0..255 integers. No round-trip of value via integers.
		PickerHueBar = 1 << 25,  // [Picker]     // ColorPicker: bar for Hue, rectangle for Sat/Value.
		PickerHueWheel = 1 << 26,  // [Picker]     // ColorPicker: wheel for Hue, triangle for Sat/Value.
		InputRGB = 1 << 27,  // [Input]      // ColorEdit, ColorPicker: input and output data in RGB format.
		InputHSV = 1 << 28,  // [Input]      // ColorEdit, ColorPicker: input and output data in HSV format.
	};
	EnumFlags(ColorEditFlags);

	void Window(const std::string& title, std::function<void()> action, WindowFlags flags = WindowFlags::None, bool* open = nullptr);
	void Child(const std::string& id, std::function<void()> action, const glm::vec2& size = { 0,0 }, bool border = false, WindowFlags = WindowFlags::None);
	void TextRaw(const std::string& text);
	void Text(const char* format, ...);
	void TextColored(const glm::vec4& color, const char* format, ...);
	void TextDisabled(const char* format, ...);
	void TextWrapped(const char* format, ...);
	void LabelText(const std::string& label, const char* format, ...);
	void BulletText(const char* format, ...);

	void Separator();
	void SameLine(float localPosX = 0.0f, float spacingW = -1.0f);
	void NewLine();
	void Spacing();
	void Dummy(const glm::vec2& size);
	void Indent(float indentW = 0.0f);
	void Unindent(float indentW = 0.0f);
	void Group(std::function<void()> action);

	bool Button(const std::string& label, const glm::vec2& size = { 0,0 });
	bool SmallButton(const std::string& label);
	bool ArrowButton(const std::string& id, Direction dir);
	void Image(const Texture2D* tex, const glm::vec2& size, const glm::vec2& uv0 = { 0,0 }, const glm::vec2& uv1 = { 1,1 }, const glm::vec4& tintCol = { 1,1,1,1 }, const glm::vec4& borderCol = { 0,0,0,0 });
	bool ImageButton(const Texture2D* tex, const glm::vec2& size, const glm::vec2& uv0 = { 0,0 }, const glm::vec2& uv1 = { 1,1 }, int framePadding = -1, const glm::vec4& bgCol = { 0, 0, 0, 0 }, const glm::vec4& tintCol = { 1, 1, 1, 1 });
	bool Checkbox(const std::string& label, bool* value);
	bool CheckboxFlags(const std::string& label, unsigned int* flags, unsigned int flagsValue);
	bool RadioButton(const std::string& label, bool active);
	void ProgressBar(float fraction, const std::string& overlay = "", const glm::vec2& sizeArg = { -1, 0 });
	void Bullet();
	void Combo(const std::string& label, const std::string& previewValue, std::function<void()> action, ComboFlags flags = ComboFlags::None);
	void Combo(const std::string& label, int& value, const std::vector<std::string>& options, ComboFlags flags = ComboFlags::None);

	bool Drag(const std::string& label, float& value, float speed = 1.0f, float min = 0.0f, float max = 0.0f, const std::string& format = "%.3f");
	bool Drag(const std::string& label, glm::vec2& value, float speed = 1.0f, float min = 0.0f, float max = 0.0f, const std::string& format = "%.3f");
	bool Drag(const std::string& label, glm::vec3& value, float speed = 1.0f, float min = 0.0f, float max = 0.0f, const std::string& format = "%.3f");
	bool Drag(const std::string& label, glm::vec4& value, float speed = 1.0f, float min = 0.0f, float max = 0.0f, const std::string& format = "%.3f");
	bool Drag(const std::string& label, int& value, float speed = 1.0f, int min = 0.0f, int max = 0.0f, const std::string& format = "%d");
	bool Drag(const std::string& label, glm::ivec2& value, float speed = 1.0f, int min = 0.0f, int max = 0.0f, const std::string& format = "%d");
	bool Drag(const std::string& label, glm::ivec3& value, float speed = 1.0f, int min = 0.0f, int max = 0.0f, const std::string& format = "%d");
	bool Drag(const std::string& label, glm::ivec4& value, float speed = 1.0f, int min = 0.0f, int max = 0.0f, const std::string& format = "%d");
	bool DragRange(const std::string& label, float& valueMin, float& valueMax, float speed = 1.0f, float min = 0.0f, float max = 0.0f, const std::string& format = "%.3f", const std::string& formatMax = nullptr);
	bool DragRange(const std::string& label, int& valueMin, int& valueMax, float speed = 1.0f, int min = 0.0f, int max = 0.0f, const std::string& format = "%d", const std::string& formatMax = nullptr);

	bool Slider(const std::string& label, float& value, float min, float max, const std::string& format = "%.3f");
	bool Slider(const std::string& label, glm::vec2& value, float min, float max, const std::string& format = "%.3f");
	bool Slider(const std::string& label, glm::vec3& value, float min, float max, const std::string& format = "%.3f");
	bool Slider(const std::string& label, glm::vec4& value, float min, float max, const std::string& format = "%.3f");
	bool Slider(const std::string& label, int& value, int min, int max, const std::string& format = "%d");
	bool Slider(const std::string& label, glm::ivec2& value, int min, int max, const std::string& format = "%d");
	bool Slider(const std::string& label, glm::ivec3& value, int min, int max, const std::string& format = "%d");
	bool Slider(const std::string& label, glm::ivec4& value, int min, int max, const std::string& format = "%d");
	bool SliderAngle(const std::string& label, float& valueRad, float minDeg = -360.0f, float maxDeg = +360.0f, const std::string& format = "%.0f deg");

	bool Input(const std::string& label, float& value, float step = 0.0f, float stepFast = 0.0f, const std::string& format = "%.3f", InputTextFlags flags = InputTextFlags::None);
	bool Input(const std::string& label, glm::vec2& value, const std::string& format = "%.3f", InputTextFlags flags = InputTextFlags::None);
	bool Input(const std::string& label, glm::vec3& value, const std::string& format = "%.3f", InputTextFlags flags = InputTextFlags::None);
	bool Input(const std::string& label, glm::vec4& value, const std::string& format = "%.3f", InputTextFlags flags = InputTextFlags::None);
	bool Input(const std::string& label, int& value, int step = 0.0f, int stepFast = 0.0f, InputTextFlags flags = InputTextFlags::None);
	bool Input(const std::string& label, ivec2& value, InputTextFlags flags = InputTextFlags::None);
	bool Input(const std::string& label, ivec3& value, InputTextFlags flags = InputTextFlags::None);
	bool Input(const std::string& label, ivec4& value, InputTextFlags flags = InputTextFlags::None);
	bool Input(const std::string& label, unsigned int& value, int step = 0.0f, int stepFast = 0.0f, InputTextFlags flags = InputTextFlags::None);
	bool Input(const std::string& label, uvec2& value, InputTextFlags flags = InputTextFlags::None);
	bool Input(const std::string& label, uvec3& value, InputTextFlags flags = InputTextFlags::None);
	bool Input(const std::string& label, uvec4& value, InputTextFlags flags = InputTextFlags::None);
	bool Input(const std::string& label, double& value, double step = 0.0f, double stepFast = 0.0f, const std::string& format = "%.6f", InputTextFlags flags = InputTextFlags::None);
	bool Input(const std::string& label, glm::dvec2& value, const std::string& format = "%.3f", InputTextFlags flags = InputTextFlags::None);
	bool Input(const std::string& label, glm::dvec3& value, const std::string& format = "%.3f", InputTextFlags flags = InputTextFlags::None);
	bool Input(const std::string& label, glm::dvec4& value, const std::string& format = "%.3f", InputTextFlags flags = InputTextFlags::None);

	bool InputText(const std::string& label, std::string& value, InputTextFlags flags = InputTextFlags::None);
	bool InputTextMultiline(const std::string& label, std::string& value, const glm::vec2& size = { 0, 0 }, InputTextFlags flags = InputTextFlags::None);
	bool InputTextWithHint(const std::string& label, const std::string& hint, std::string& value, InputTextFlags flags = InputTextFlags::None);

	bool ColorEdit(const std::string& label, glm::vec3& value, ColorEditFlags flags = ColorEditFlags::None);
	bool ColorEdit(const std::string& label, glm::vec4& value, ColorEditFlags flags = ColorEditFlags::None);
	bool ColorPicker(const std::string& label, glm::vec3& value, ColorEditFlags flags = ColorEditFlags::None);
	bool ColorPicker(const std::string& label, glm::vec4& value, ColorEditFlags flags = ColorEditFlags::None);
	bool ColorButton(const std::string& descId, const glm::vec4& col, ColorEditFlags flags = ColorEditFlags::None, const glm::vec2& size = { 0, 0 });  // display a colored square/button, hover for details, return true when pressed.

	void TreeNode(const std::string& label, std::function<void()> action);
	bool CollapsingHeader(const std::string& label);
}