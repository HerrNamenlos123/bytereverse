#pragma once

#include "pch.h"
#include "_BaseUI.h"

class WorkerUI : public _BaseUI {
public:
	WorkerUI(const std::string profileName, const std::string sourceFile, const std::string targetFile);
	~WorkerUI();

	void update();
	void render();
	void onEvent(sf::Event event);
	glm::ivec2 wantedWindowSize();

	inline static const glm::ivec2 defaultWindowSize = { 360, 80 };

private:
	void OnUpdate();
	void OnRender();

private:
	Battery::ImGuiPanel panel;
	std::thread workerThread;
	std::atomic<bool> workSucceeded;
	std::string workErrorMessage;
	std::atomic<bool> threadReadyForJoining = false;
	bool threadJoined = false;

	float fadeTime = 4.f;
	float fadeStartTime = 0.f;
	float minimumWorkTime = 0.2f;

	sf::Texture iconTexture;

	std::string profile;
	std::string source;
	std::string target;

};
