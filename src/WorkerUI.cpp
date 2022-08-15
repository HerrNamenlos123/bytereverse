
#include "WorkerUI.h"
#include "workerThread.h"
#include "GlobalResources.h"

WorkerUI::WorkerUI(const std::string profileName, const std::string sourceFile, const std::string targetFile) : panel([&] { OnUpdate(); }, [&] { OnRender(); }) {
    canBeClosed = false;

    profile = profileName;
    source = sourceFile;
    target = targetFile;

	workerThread = std::thread([&] { 
        LOG_INFO("Deploying work from worker thread");
		
        auto start = Battery::GetRuntime();
		workSucceeded = doByteReverseWork(workErrorMessage, profile, source, target);
        Battery::Sleep(minimumWorkTime - (Battery::GetRuntime() - start).asSeconds());
		
        LOG_INFO("Work is done, worker thread is ready to be joined");
		threadReadyForJoining = true;
	});
}

WorkerUI::~WorkerUI() {
    if (!threadJoined) {
        LOG_WARN("~WorkerUI(): workerThread has not been joined yet, waiting...");
        workerThread.join();
        LOG_WARN("~WorkerUI(): Thread has been joined");
    }
}



void WorkerUI::update() {
	panel.position = { 0, 0 };
	panel.size = { Battery::GetApp().window.getSize().x, Battery::GetApp().window.getSize().y };

	if (threadReadyForJoining && !threadJoined) {
		workerThread.join();
		threadJoined = true;
		canBeClosed = true;

        if (!workSucceeded) {
            Battery::MessageBoxError("Failed to convert bitstream: " + workErrorMessage);
            shouldBeClosed = true;
        }

		fadeStartTime = Battery::GetRuntime().asSeconds();
		Battery::SetWindowTransparent(Battery::GetApp().window, true);
	}

	panel.update();
}

void WorkerUI::render() {

	if (fadeStartTime != 0) {
		float now = Battery::GetRuntime().asSeconds();
		float alpha = Battery::Map<float>(now - fadeStartTime, 0, fadeTime, 255, 0);
		if (alpha > 0) {
			Battery::SetWindowAlpha(Battery::GetApp().window, alpha);
		}
		else {
			Battery::GetApp().window.setVisible(false);
			fadeStartTime = 0;
            shouldBeClosed = true;
		}
	}

	panel.render();
}

void WorkerUI::onEvent(sf::Event event) {

}

glm::ivec2 WorkerUI::wantedWindowSize() {
    return defaultWindowSize;
}




void WorkerUI::OnUpdate() {

}

void WorkerUI::OnRender() {
    auto& window = Battery::GetApp().window;

    int offset = ImGui::GetStyle().WindowPadding.y;
    ImGui::Columns(2, 0, false);
    ImGui::SetColumnWidth(0, ImGui::GetWindowHeight() - offset);
    ImGui::Image((ImTextureID)RES->ArduinoIconTexture64.getNativeHandle(), ImVec2(ImGui::GetWindowHeight() - offset * 2, ImGui::GetWindowHeight() - offset * 2));
    ImGui::NextColumn();
    ImGui::PushFont(Fonts::robotoWorker);
    ImGui::Text("Profile: [%s]", profile.c_str());
    ImGui::Text("Input:    %s", shortenFilename(source).c_str());
    ImGui::Text("Output: %s", shortenFilename(target).c_str());
    ImGui::EndColumns();

    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 58);
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() / 2 - 10 - offset);

    if (!threadJoined) {
        Battery::DrawImGuiSpinner("Loading", 16, 6, 0xFF00FF00);
    }
    else {
        ImGui::PushFont(Fonts::fontAwesomeHugeCheckMark);
        if (workSucceeded) {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "\xef\x80\x8c");	// Green check mark
        }
        else {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "\xef\x80\x8d");	// Red cross
        }
        ImGui::PopFont();
    }

    ImGui::PopFont();
}
