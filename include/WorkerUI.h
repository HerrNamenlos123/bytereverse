#pragma once

#include "pch.h"
#include "_BaseUI.h"

class WorkerUI : public _BaseUI {
public:
	WorkerUI();
	~WorkerUI();

	void update();
	void render();

	std::string profile = "<ProFiLeNaMe>";
	std::string source = "<SoUrCeFiLe>";
	std::string target = "<TaRgEtFiLe>";

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

	sf::Texture iconTexture;

};
