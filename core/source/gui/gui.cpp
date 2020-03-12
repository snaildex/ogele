#include <stdafx.h>
#include <ogele.h>


#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_stdlib.h"

#define CAST(what, type) reinterpret_cast< type &>( what )

using namespace std;
using namespace glm;
using namespace ImGui;
namespace ogele::GUI {

	namespace Internal {
		void Init(GLFWwindow* window) {
			CreateContext();
			ImGui_ImplGlfw_InitForOpenGL(window, false);
			ImGui_ImplOpenGL3_Init();
			StyleColorsDark();
			ImGuiIO& io = GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.IniFilename = nullptr;
		}
		void Shutdown() {
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			DestroyContext();
		}
		void NewFrame() {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			SetNextWindowPos({ 0,0 }, ImGuiCond_Always);
			auto res = Application::GetInstance()->GetResolution();
			SetNextWindowSize(ImVec2(res.x, res.y), ImGuiCond_Always);
			Begin("##Main", nullptr,
				ImGuiWindowFlags_NoBackground |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoInputs |
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoBringToFrontOnFocus |
				ImGuiWindowFlags_NoSavedSettings
			);
		}
		void Render()
		{
			End();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
		}
	}

	void Window(const std::string& title, std::function<void()> action, WindowFlags flags, bool* open) {
		Begin(title.data(), open, CAST(flags, ImGuiWindowFlags));
		action();
		End();
	}
	void Child(const std::string& id, std::function<void()> action, const glm::vec2& size, bool border, WindowFlags flags) {
		BeginChild(id.data(), CAST(size, const ImVec2), border, CAST(flags, ImGuiWindowFlags));
		action();
		EndChild();
	}
	void TextRaw(const std::string& text) {
		TextUnformatted(text.data(), text.data() + text.size());
	}
	void Text(const char* format, ...) {
		va_list args;
		va_start(args, format);
		TextV(format, args);
		va_end(args);
	}
	void TextColored(const glm::vec4& color, const char* format, ...) {
		va_list args;
		va_start(args, format);
		TextColoredV(CAST(color, const ImVec4), format, args);
		va_end(args);
	}
	void TextDisabled(const char* format, ...) {
		va_list args;
		va_start(args, format);
		TextDisabledV(format, args);
		va_end(args);
	}
	void TextWrapped(const char* format, ...) {
		va_list args;
		va_start(args, format);
		TextWrappedV(format, args);
		va_end(args);
	}
	void LabelText(const std::string& label, const char* format, ...) {
		va_list args;
		va_start(args, format);
		LabelTextV(label.data(), format, args);
		va_end(args);
	}
	void BulletText(const char* format, ...) {
		va_list args;
		va_start(args, format);
		BulletTextV(format, args);
		va_end(args);
	}

	void Separator() {
		ImGui::Separator();
	}
	void SameLine(float localPosX, float spacingW) {
		ImGui::SameLine(localPosX, spacingW);
	}
	void NewLine() {
		ImGui::NewLine();
	}
	void Spacing() {
		ImGui::Spacing();
	}
	void Dummy(const glm::vec2& size) {
		ImGui::Dummy(CAST(size, const ImVec2));
	}
	void Indent(float indentW) {
		ImGui::Indent(indentW);
	}
	void Unindent(float indentW) {
		ImGui::Unindent(indentW);
	}
	void Group(std::function<void()> action) {
		ImGui::BeginGroup();
		action();
		ImGui::EndGroup();
	}

	bool Button(const std::string& label, const glm::vec2& size) {
		return ImGui::Button(label.data(), CAST(size, const ImVec2));
	}
	bool SmallButton(const std::string& label) {
		return ImGui::SmallButton(label.data());
	}
	bool ArrowButton(const std::string& id, Direction dir) {
		return ImGui::ArrowButton(id.data(), CAST(dir, ImGuiDir));
	}
	void Image(const Texture2D* tex, const glm::vec2& size, const glm::vec2& uv0, const glm::vec2& uv1, const glm::vec4& tintCol, const glm::vec4& borderCol) {
		ImGui::Image((ImTextureID)tex->GetHandle(), CAST(size, const ImVec2), CAST(uv0, const ImVec2), CAST(uv1, const ImVec2), CAST(tintCol, const ImVec4), CAST(borderCol, const ImVec4));
	}
	bool ImageButton(const Texture2D* tex, const glm::vec2& size, const glm::vec2& uv0, const glm::vec2& uv1, int framePadding, const glm::vec4& bgCol, const glm::vec4& tintCol) {
		return ImGui::ImageButton((ImTextureID)tex->GetHandle(), CAST(size, const ImVec2), CAST(uv0, const ImVec2), CAST(uv1, const ImVec2), framePadding, CAST(bgCol, const ImVec4), CAST(tintCol, const ImVec4));
	}
	bool Checkbox(const std::string& label, bool* value) {
		return ImGui::Checkbox(label.data(), value);
	}
	bool CheckboxFlags(const std::string& label, unsigned int* flags, unsigned int flagsValue) {
		return ImGui::CheckboxFlags(label.data(), flags, flagsValue);
	}
	bool RadioButton(const std::string& label, bool active) {
		return ImGui::RadioButton(label.data(), active);
	}
	void ProgressBar(float fraction, const std::string& overlay, const glm::vec2& sizeArg) {
		ImGui::ProgressBar(fraction, CAST(sizeArg, const ImVec2), overlay.data());
	}
	void Bullet() {
		ImGui::Bullet();
	}
	void Combo(const std::string& label, const std::string& previewValue, std::function<void()> action, ComboFlags flags) {
		if (ImGui::BeginCombo(label.data(), previewValue.data(), CAST(flags, ImGuiComboFlags))) {
			action();
			ImGui::EndCombo();
		}
	}
	void Combo(const std::string& label, int& value, const std::vector<std::string>& options, ComboFlags flags) {
		if (ImGui::BeginCombo(label.data(), options[value].data(), CAST(flags, ImGuiComboFlags))) {
			for (int i = 0; i < options.size(); ++i)
				if (ImGui::Selectable(options[i].data(), i == value))
					value = i;
			ImGui::EndCombo();
		}
	}

	bool Drag(const std::string& label, float& value, float speed, float min, float max, const std::string& format) {
		return ImGui::DragFloat(label.data(), &value, speed, min, max, format.data());
	}
	bool Drag(const std::string& label, glm::vec2& value, float speed, float min, float max, const std::string& format) {
		return ImGui::DragFloat2(label.data(), glm::value_ptr(value), speed, min, max, format.data());
	}
	bool Drag(const std::string& label, glm::vec3& value, float speed, float min, float max, const std::string& format) {
		return ImGui::DragFloat3(label.data(), glm::value_ptr(value), speed, min, max, format.data());
	}
	bool Drag(const std::string& label, glm::vec4& value, float speed, float min, float max, const std::string& format) {
		return ImGui::DragFloat4(label.data(), glm::value_ptr(value), speed, min, max, format.data());
	}
	bool Drag(const std::string& label, int& value, float speed, int min, int max, const std::string& format) {
		return ImGui::DragInt(label.data(), &value, speed, min, max, format.data());
	}
	bool Drag(const std::string& label, glm::ivec2& value, float speed, int min, int max, const std::string& format) {
		return ImGui::DragInt2(label.data(), glm::value_ptr(value), speed, min, max, format.data());
	}
	bool Drag(const std::string& label, glm::ivec3& value, float speed, int min, int max, const std::string& format) {
		return ImGui::DragInt3(label.data(), glm::value_ptr(value), speed, min, max, format.data());
	}
	bool Drag(const std::string& label, glm::ivec4& value, float speed, int min, int max, const std::string& format) {
		return ImGui::DragInt4(label.data(), glm::value_ptr(value), speed, min, max, format.data());
	}
	bool DragRange(const std::string& label, float& valueMin, float& valueMax, float speed, float min, float max, const std::string& format, const std::string& formatMax) {
		return ImGui::DragFloatRange2(label.data(), &valueMin, &valueMax, speed, min, max, format.data(), formatMax.data());
	}
	bool DragRange(const std::string& label, int& valueMin, int& valueMax, float speed, int min, int max, const std::string& format, const std::string& formatMax) {
		return ImGui::DragIntRange2(label.data(), &valueMin, &valueMax, speed, min, max, format.data(), formatMax.data());
	}

	bool Slider(const std::string& label, float& value, float min, float max, const std::string& format) {
		return ImGui::SliderFloat(label.data(), &value, min, max, format.data());
	}
	bool Slider(const std::string& label, glm::vec2& value, float min, float max, const std::string& format) {
		return ImGui::SliderFloat2(label.data(), glm::value_ptr(value), min, max, format.data());
	}
	bool Slider(const std::string& label, glm::vec3& value, float min, float max, const std::string& format) {
		return ImGui::SliderFloat3(label.data(), glm::value_ptr(value), min, max, format.data());
	}
	bool Slider(const std::string& label, glm::vec4& value, float min, float max, const std::string& format) {
		return ImGui::SliderFloat4(label.data(), glm::value_ptr(value), min, max, format.data());
	}
	bool Slider(const std::string& label, int& value, int min, int max, const std::string& format) {
		return ImGui::SliderInt(label.data(), &value, min, max, format.data());
	}
	bool Slider(const std::string& label, glm::ivec2& value, int min, int max, const std::string& format) {
		return ImGui::SliderInt2(label.data(), glm::value_ptr(value), min, max, format.data());
	}
	bool Slider(const std::string& label, glm::ivec3& value, int min, int max, const std::string& format) {
		return ImGui::SliderInt3(label.data(), glm::value_ptr(value), min, max, format.data());
	}
	bool Slider(const std::string& label, glm::ivec4& value, int min, int max, const std::string& format) {
		return ImGui::SliderInt4(label.data(), glm::value_ptr(value), min, max, format.data());
	}
	bool SliderAngle(const std::string& label, float& valueRad, float minDeg, float maxDeg, const std::string& format) {
		return ImGui::SliderAngle(label.data(), &valueRad, minDeg, maxDeg, format.data());
	}

	bool Input(const std::string& label, float& value, float step, float stepFast, const std::string& format, InputTextFlags flags) {
		return ImGui::InputFloat(label.data(), &value, step, stepFast, format.data(), CAST(flags, ImGuiInputTextFlags));
	}
	bool Input(const std::string& label, glm::vec2& value, const std::string& format, InputTextFlags flags) {
		return ImGui::InputFloat2(label.data(), glm::value_ptr(value), format.data(), CAST(flags, ImGuiInputTextFlags));
	}
	bool Input(const std::string& label, glm::vec3& value, const std::string& format, InputTextFlags flags) {
		return ImGui::InputFloat3(label.data(), glm::value_ptr(value), format.data(), CAST(flags, ImGuiInputTextFlags));
	}
	bool Input(const std::string& label, glm::vec4& value, const std::string& format, InputTextFlags flags) {
		return ImGui::InputFloat4(label.data(), glm::value_ptr(value), format.data(), CAST(flags, ImGuiInputTextFlags));
	}
	bool Input(const std::string& label, int& value, int step, int stepFast, InputTextFlags flags) {
		return ImGui::InputInt(label.data(), &value, step, stepFast, CAST(flags, ImGuiInputTextFlags));
	}
	bool Input(const std::string& label, ivec2& value, InputTextFlags flags) {
		return ImGui::InputInt2(label.data(), glm::value_ptr(value), CAST(flags, ImGuiInputTextFlags));
	}
	bool Input(const std::string& label, ivec3& value, InputTextFlags flags) {
		return ImGui::InputInt3(label.data(), glm::value_ptr(value), CAST(flags, ImGuiInputTextFlags));
	}
	bool Input(const std::string& label, ivec4& value, InputTextFlags flags) {
		return ImGui::InputInt4(label.data(), glm::value_ptr(value), CAST(flags, ImGuiInputTextFlags));
	}
	bool Input(const std::string& label, unsigned int& value, int step, int stepFast, InputTextFlags flags) {
		return ImGui::InputUInt(label.data(), &value, step, stepFast, CAST(flags, ImGuiInputTextFlags));
	}
	bool Input(const std::string& label, uvec2& value, InputTextFlags flags) {
		return ImGui::InputUInt2(label.data(), glm::value_ptr(value), CAST(flags, ImGuiInputTextFlags));
	}
	bool Input(const std::string& label, uvec3& value, InputTextFlags flags) {
		return ImGui::InputUInt3(label.data(), glm::value_ptr(value), CAST(flags, ImGuiInputTextFlags));
	}
	bool Input(const std::string& label, uvec4& value, InputTextFlags flags) {
		return ImGui::InputUInt4(label.data(), glm::value_ptr(value), CAST(flags, ImGuiInputTextFlags));
	}
	bool Input(const std::string& label, double& value, double step, double stepFast, const std::string& format, InputTextFlags flags) {
		return ImGui::InputDouble(label.data(), &value, step, stepFast, format.data(), CAST(flags, ImGuiInputTextFlags));
	}
	bool Input(const std::string& label, glm::dvec2& value, const std::string& format, InputTextFlags flags) {
		return ImGui::InputDouble2(label.data(), glm::value_ptr(value), format.data(), CAST(flags, ImGuiInputTextFlags));
	}
	bool Input(const std::string& label, glm::dvec3& value, const std::string& format, InputTextFlags flags) {
		return ImGui::InputDouble3(label.data(), glm::value_ptr(value), format.data(), CAST(flags, ImGuiInputTextFlags));
	}
	bool Input(const std::string& label, glm::dvec4& value, const std::string& format, InputTextFlags flags) {
		return ImGui::InputDouble4(label.data(), glm::value_ptr(value), format.data(), CAST(flags, ImGuiInputTextFlags));
	}

	bool InputText(const std::string& label, std::string& value, InputTextFlags flags) {
		return ImGui::InputText(label.data(), &value, CAST(flags, ImGuiInputTextFlags));
	}
	bool InputTextMultiline(const std::string& label, std::string& value, const glm::vec2& size, InputTextFlags flags) {
		return ImGui::InputTextMultiline(label.data(), &value, CAST(size, const ImVec2), CAST(flags, ImGuiInputTextFlags));
	}
	bool InputTextWithHint(const std::string& label, const std::string& hint, std::string& value, InputTextFlags flags) {
		return ImGui::InputTextWithHint(label.data(), hint.data(), &value, CAST(flags, ImGuiInputTextFlags));
	}

	bool ColorEdit(const std::string& label, glm::vec3& value, ColorEditFlags flags) {
		return ImGui::ColorEdit3(label.data(), glm::value_ptr(value), CAST(flags, ImGuiColorEditFlags));
	}
	bool ColorEdit(const std::string& label, glm::vec4& value, ColorEditFlags flags) {
		return ImGui::ColorEdit4(label.data(), glm::value_ptr(value), CAST(flags, ImGuiColorEditFlags));
	}
	bool ColorPicker(const std::string& label, glm::vec3& value, ColorEditFlags flags) {
		return ImGui::ColorPicker3(label.data(), glm::value_ptr(value), CAST(flags, ImGuiColorEditFlags));
	}
	bool ColorPicker(const std::string& label, glm::vec4& value, ColorEditFlags flags) {
		return ImGui::ColorPicker4(label.data(), glm::value_ptr(value), CAST(flags, ImGuiColorEditFlags));
	}
	bool ColorButton(const std::string& descId, const glm::vec4& col, ColorEditFlags flags, const glm::vec2& size) {
		return ImGui::ColorButton(descId.data(), CAST(col, const ImVec4), CAST(flags, ImGuiColorEditFlags), CAST(size, const ImVec2));
	}

	void TreeNode(const std::string& label, std::function<void()> action) {
		if (ImGui::TreeNode(label.data())) {
			action();
			ImGui::TreePop();
		}
	}
	bool CollapsingHeader(const std::string& label) {
		return ImGui::CollapsingHeader(label.data());
	}

	template<> void Internal::Property(const std::string& name, int& value) { Input(name, value); }
	template<> void Internal::Property(const std::string& name, unsigned int& value) { Input(name, value); }
	template<> void Internal::Property(const std::string& name, float& value) { Input(name, value); }
	template<> void Internal::Property(const std::string& name, double& value) { Input(name, value); }
	template<> void Internal::Property(const std::string& name, glm::vec2& value) { Input(name, value); }
	template<> void Internal::Property(const std::string& name, glm::vec3& value) { Input(name, value); }
	template<> void Internal::Property(const std::string& name, glm::vec4& value) { Input(name, value); }
	template<> void Internal::Property(const std::string& name, glm::ivec2& value) { Input(name, value); }
	template<> void Internal::Property(const std::string& name, glm::ivec3& value) { Input(name, value); }
	template<> void Internal::Property(const std::string& name, glm::ivec4& value) { Input(name, value); }
	template<> void Internal::Property(const std::string& name, glm::uvec2& value) { Input(name, value); }
	template<> void Internal::Property(const std::string& name, glm::uvec3& value) { Input(name, value); }
	template<> void Internal::Property(const std::string& name, glm::uvec4& value) { Input(name, value); }
	template<> void Internal::Property(const std::string& name, glm::dvec2& value) { Input(name, value); }
	template<> void Internal::Property(const std::string& name, glm::dvec3& value) { Input(name, value); }
	template<> void Internal::Property(const std::string& name, glm::dvec4& value) { Input(name, value); }

	template<> void Internal::Property(const std::string& name, glm::mat2x2& value) { LabelText(name, "%s", typeid(value).name()); }
	template<> void Internal::Property(const std::string& name, glm::mat2x3& value) { LabelText(name, "%s", typeid(value).name()); }
	template<> void Internal::Property(const std::string& name, glm::mat2x4& value) { LabelText(name, "%s", typeid(value).name()); }
	template<> void Internal::Property(const std::string& name, glm::mat3x2& value) { LabelText(name, "%s", typeid(value).name()); }
	template<> void Internal::Property(const std::string& name, glm::mat3x3& value) { LabelText(name, "%s", typeid(value).name()); }
	template<> void Internal::Property(const std::string& name, glm::mat3x4& value) { LabelText(name, "%s", typeid(value).name()); }
	template<> void Internal::Property(const std::string& name, glm::mat4x2& value) { LabelText(name, "%s", typeid(value).name()); }
	template<> void Internal::Property(const std::string& name, glm::mat4x3& value) { LabelText(name, "%s", typeid(value).name()); }
	template<> void Internal::Property(const std::string& name, glm::mat4x4& value) { LabelText(name, "%s", typeid(value).name()); }

	template<> void Internal::Property(const std::string& name, glm::dmat2x2& value) { LabelText(name, "%s", typeid(value).name()); }
	template<> void Internal::Property(const std::string& name, glm::dmat2x3& value) { LabelText(name, "%s", typeid(value).name()); }
	template<> void Internal::Property(const std::string& name, glm::dmat2x4& value) { LabelText(name, "%s", typeid(value).name()); }
	template<> void Internal::Property(const std::string& name, glm::dmat3x2& value) { LabelText(name, "%s", typeid(value).name()); }
	template<> void Internal::Property(const std::string& name, glm::dmat3x3& value) { LabelText(name, "%s", typeid(value).name()); }
	template<> void Internal::Property(const std::string& name, glm::dmat3x4& value) { LabelText(name, "%s", typeid(value).name()); }
	template<> void Internal::Property(const std::string& name, glm::dmat4x2& value) { LabelText(name, "%s", typeid(value).name()); }
	template<> void Internal::Property(const std::string& name, glm::dmat4x3& value) { LabelText(name, "%s", typeid(value).name()); }
	template<> void Internal::Property(const std::string& name, glm::dmat4x4& value) { LabelText(name, "%s", typeid(value).name()); }
}