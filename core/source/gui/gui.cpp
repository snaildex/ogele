#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele::GUI {
#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

	nk_color Color(const vec4& color) {
		nk_color res;
		res.r = static_cast<nk_byte>(color.r * 255);
		res.g = static_cast<nk_byte>(color.g * 255);
		res.b = static_cast<nk_byte>(color.b * 255);
		res.a = static_cast<nk_byte>(color.a * 255);
		return res;
	}
	const nk_colorf& Colorf(const vec4& color) {
		return reinterpret_cast<const nk_colorf&>(color);
	}

	nk_context *ctx;
	nk_font_atlas *fontAtlas;

	namespace Internal {
		void Init(GLFWwindow* window) {
			ctx = nk_glfw3_init(window, NK_GLFW3_DEFAULT);
			nk_glfw3_font_stash_begin(&fontAtlas);
			nk_font *font = nk_font_atlas_add_default(fontAtlas, 13, nullptr);
			nk_glfw3_font_stash_end();
			nk_style_set_font(ctx, &font->handle);
		}
		void Shutdown() {
			nk_glfw3_shutdown();
		}
		void NewFrame() {
			nk_glfw3_new_frame();
		}
		void Render() {
			nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
		}
	}

	namespace Layout {
		void SetMinRowHeight(float height) { nk_layout_set_min_row_height(ctx, height); }
		void ResetMinRowHeight() { nk_layout_reset_min_row_height(ctx); }
		void RowDynamic(int columns, float height) { nk_layout_row_dynamic(ctx, height, columns); }
		void RowDynamic(int columns, std::function<void(void)> action, float height) {
			nk_layout_row_begin(ctx, NK_DYNAMIC, height, columns);
			action();
			nk_layout_row_end(ctx);
		}
		void RowStatic(int columns, int columnWidth, float height) { nk_layout_row_static(ctx, height, columnWidth, columns); }
		void RowStatic(int columns, std::function<void(void)> action, float height) {
			nk_layout_row_begin(ctx, NK_STATIC, height, columns);
			action();
			nk_layout_row_end(ctx);
		}
		void RowPush(float value) { nk_layout_row_push(ctx, value); }
		void RowTemplate(std::function<void(void)> action, float height) {
			nk_layout_row_template_begin(ctx, height);
			action();
			nk_layout_row_template_end(ctx);
		}
		void TemplateDynamic() { nk_layout_row_template_push_dynamic(ctx); }
		void TemplateVariable(float minWidth) { nk_layout_row_template_push_variable(ctx, minWidth); }
		void TemplateStatic(float width) { nk_layout_row_template_push_static(ctx, width); }
		void SpaceDynamic(int widgets, std::function<void(void)> action, float height) {
			nk_layout_space_begin(ctx, NK_DYNAMIC, height, widgets);
			action();
			nk_layout_space_end(ctx);
		}
		void SpaceStatic(int widgets, std::function<void(void)> action, float height) {
			nk_layout_space_begin(ctx, NK_STATIC, height, widgets);
			action();
			nk_layout_space_end(ctx);
		}
		void SpacePush(const vec2& position, const vec2& size) { nk_layout_space_push(ctx, nk_rect(position.x, position.y, size.x, size.y)); }
	}

	void Window(const std::string& caption, WindowFlags flags, const vec2& position, const vec2& size, std::function<void(void)> action) {
		if (nk_begin(ctx, caption.data(), nk_rect(position.x, position.y, size.x, size.y), reinterpret_cast<nk_window_flags&>(flags)))
			action();
		nk_end(ctx);
	}
	void Group(const std::string& title, WindowFlags flags, std::function<void(void)> action) {
		if (nk_group_begin(ctx, title.data(), reinterpret_cast<nk_window_flags&>(flags))) {
			action();
			nk_group_end(ctx);
		}
	}
	void Tree(const std::string& title, const void* hash, int hasSize, int seed, std::function<void(void)> action, TreeType type, CollapseState state) {
		if (nk_tree_push_hashed(
			ctx,
			reinterpret_cast<nk_tree_type&>(type),
			title.c_str(),
			reinterpret_cast<nk_collapse_states&>(state),
			reinterpret_cast<const char*>(hash),
			hasSize,
			seed)) {
			action();
			nk_tree_pop(ctx);
		}
	}
	void Label(const std::string& text, TextAlign align) {
		nk_label(ctx, text.data(), reinterpret_cast<nk_text_align&>(align));
	}
	void LabelColored(const std::string& text, const glm::vec4& color, TextAlign align) {
		nk_label_colored(ctx, text.data(), reinterpret_cast<nk_text_align&>(align), Color(color));
	}
	void Image(const Texture2D* image) {
		nk_image(ctx, nk_image_id(static_cast<int>(image->GetHandle())));
	}
}