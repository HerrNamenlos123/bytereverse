
#include "WorkerUI.h"
#include "workerThread.h"
#include "GlobalResources.h"

WorkerUI::WorkerUI() : panel([&] { OnUpdate(); }, [&] { OnRender(); }) {
	workerThread = std::thread([&] { 
		workSucceeded = doByteReverseWork(workErrorMessage);
		threadReadyForJoining = true;
	});
}

WorkerUI::~WorkerUI() {
	if (!threadJoined)
		workerThread.join();
}



void WorkerUI::update() {
	panel.position = { 0, 0 };
	panel.size = { Battery::GetApp().window.getSize().x, Battery::GetApp().window.getSize().y };

	if (threadReadyForJoining && !threadJoined) {
		workerThread.join();
		threadJoined = true;
		canBeClosed = true;

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




void WorkerUI::OnUpdate() {

}

void WorkerUI::OnRender() {
    auto& window = Battery::GetApp().window;

    int offset = ImGui::GetStyle().WindowPadding.y;
    ImGui::Columns(2, 0, false);
    ImGui::SetColumnWidth(0, ImGui::GetWindowHeight() - offset);
    ImGui::Image((ImTextureID)RES->ArduinoIconTexture.getNativeHandle(), ImVec2(ImGui::GetWindowHeight() - offset * 2, ImGui::GetWindowHeight() - offset * 2));
    ImGui::NextColumn();
    ImGui::PushFont(Fonts::robotoSmall);
    ImGui::Text("Profile: %s", profile.c_str());
    ImGui::Text("Source: %s", source.c_str());
    ImGui::Text("Target: %s", target.c_str());
    ImGui::EndColumns();

    int max = 0;
    ImVec2 p = ImGui::CalcTextSize(profile.c_str());
    max = std::max<int>(max, p.x);
    p = ImGui::CalcTextSize(source.c_str());
    max = std::max<int>(max, p.x);
    p = ImGui::CalcTextSize(target.c_str());
    max = std::max<int>(max, p.x);

    int width = std::max<int>(originalSize.x, max + ImGui::GetWindowHeight() * 2 + 50);
    if (width != window.getSize().x)
        window.setSize({ (uint16_t)width, window.getSize().y });

    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 58);
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() / 2 - 10 - offset);

    if (!threadJoined) {
        Battery::DrawImGuiSpinner("Loading", 16, 6, 0xFF00FF00);
    }
    else {
        ImGui::PushFont(Fonts::fontAwesome);
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "\xef\x80\x8c");	// Green check mark
        ImGui::PopFont();
    }

    ImGui::PopFont();
}
