#pragma once

#include "pch.h"
#include "_BaseUI.h"

struct OptionsFile {

	struct Profile {
		std::string name;
		std::string sourcePath;
		std::string targetPath;

		template<class Archive>
		void serialize(Archive& archive) {
			archive( name, sourcePath, targetPath );
		}
	};

	std::vector<Profile> profiles;
	size_t activeProfile = 0;
	bool autostart = true;
	bool installRegistered = false;
	std::string installationUUID;
	
	static void loadOptions();
	static void writeOptions();

	template<class Archive>
	void serialize(Archive& archive) {
		archive( profiles, activeProfile, autostart, installRegistered, installationUUID);
	}
};

extern OptionsFile optionsFile;

class OptionsUI : public _BaseUI {
public:
	OptionsUI();
	~OptionsUI();

	void update();
	void render();
	void onEvent(sf::Event event);
	glm::ivec2 wantedWindowSize();

	inline static const glm::ivec2 defaultWindowSize = { 350, 250 };

private:
	OptionsFile::Profile getActiveProfile();
	void addProfile(const std::string& name);
	static bool checkSourceFile(const std::string& path);
	static bool checkTargetFile(const std::string& path);
	static std::string openFileDialog();
	static std::string saveFileDialog();
	void OnUpdate();
	void OnRender();

private:
	Battery::ImGuiPanel panel;
	Battery::DropdownMenu profilesDropdown;
	size_t noFocusCount = 0;
	bool ignoreFocusLoss = false;
	char textInputBuffer[64];
	char feedbackTextInputBuffer[1024];

	std::string sourceFileChecked;
	bool sourceFileValid = false;
	std::string targetFileChecked;
	bool targetFileValid = false;

	bool chooseInput = false;
	bool chooseOutput = false;
};
