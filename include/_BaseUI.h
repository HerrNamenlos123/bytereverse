#pragma once

#include "pch.h"
#include "Fonts.h"

namespace ImGui {

	static void HelpMarker(const char* desc, ImFont* font = nullptr) {
		if (font) ImGui::PushFont(font);
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
		if (font) ImGui::PopFont();
	}

	static void HelperPopup(const char* desc, ImFont* font = nullptr) {
		if (font) ImGui::PushFont(font);
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
		if (font) ImGui::PopFont();
	}
}

std::string shortenFilename(const std::string& filename);

class _BaseUI {
public:
	_BaseUI() {}
	virtual ~_BaseUI() {}

	void setupWindow(uint8_t alpha = 255);
	void moveWindowDelta(int dx, int dy);

	bool canBeClosed = true;
	bool shouldBeClosed = false;
	glm::ivec2 originalSize = { 0, 0 };
	
	virtual void update() = 0;
	virtual void render() = 0;
	virtual void onEvent(sf::Event event) = 0;
	virtual glm::ivec2 wantedWindowSize() = 0;

private:

};
