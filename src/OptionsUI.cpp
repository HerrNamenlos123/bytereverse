
#include "OptionsUI.h"
#include "GlobalResources.h"
#include "TelegramBot.h"
#include "cereal.h"
#include "nfd.h"
#include "Shortcut.h"

OptionsFile optionsFile;

void SetupAutostart() {
    std::string shortcutPath = RES->autostartShortcutDir + "/" + Battery::GetExecutableName() + ".lnk";

    if (optionsFile.autostart) {
        if (!Battery::FileExists(shortcutPath)) {
            LOG_INFO("Autostart enabled, shortcut must be created");
            if (!CreateShortcut(shortcutPath, Battery::GetExecutablePath())) {
                LOG_ERROR("Failed to create autostart shortcut: {} to {}", shortcutPath, Battery::GetExecutablePath());
            }
        }
    }
    else {
        if (Battery::FileExists(shortcutPath)) {
            LOG_INFO("Autostart disabled, shortcut must be deleted");
            if (!Battery::RemoveFile(shortcutPath)) {
                LOG_ERROR("Failed to remove autostart shortcut: '{}'", shortcutPath);
            }
        }
    }
}

void OptionsFile::loadOptions() {
    LOG_DEBUG("Loading options from options file: {}", RES->optionsFilePath);

    try {

        std::ifstream file(RES->optionsFilePath);
        if (!file.is_open()) {
            LOG_ERROR("Failed to open options file: {}", RES->optionsFilePath);
            return;
        }

        // Moving object to heap to reduce the stack size for this function
        std::unique_ptr<cereal::XMLInputArchive> iarchive = std::make_unique<cereal::XMLInputArchive>(file);
        (*iarchive.get())(optionsFile);
        iarchive.reset();
    }
    catch (const std::exception& e) {
        LOG_ERROR("Failed to open options file: {}\nReason: XML parsing threw an exception: '{}'", RES->optionsFilePath, e.what());
    }

    SetupAutostart();
}

void OptionsFile::writeOptions() {
    LOG_DEBUG("Writing options to options file: {}", RES->optionsFilePath);

    std::ofstream file(RES->optionsFilePath);

    // Moving object to heap to reduce the stack size for this function
    std::unique_ptr<cereal::XMLOutputArchive> oarchive = std::make_unique<cereal::XMLOutputArchive>(file);
    (*oarchive.get())(optionsFile);
    oarchive.reset();

    SetupAutostart();
}



OptionsUI::OptionsUI() : panel([&] { OnUpdate(); }, [&] { OnRender(); }) {
    OptionsFile::loadOptions();
    memset(textInputBuffer, 0, sizeof(textInputBuffer));
}

OptionsUI::~OptionsUI() {
    OptionsFile::writeOptions();
}



void OptionsUI::update() {
    panel.position = { 0, 0 };
    panel.size = { Battery::GetApp().window.getSize().x, Battery::GetApp().window.getSize().y };
    panel.update();
}

void OptionsUI::render() {
    panel.render();
}

void OptionsUI::onEvent(sf::Event event) {
    if (event.type == sf::Event::LostFocus) {

        if (ignoreFocusLoss) {
            ignoreFocusLoss = false;
            LOG_WARN("[OptionsUI] Ignoring focus loss and refocussing");
            Battery::SetWindowFocus(Battery::GetApp().window);
            return;
        }

        LOG_INFO("[OptionsUI] Window lost focus, requesting close");
        shouldBeClosed = true;
    }
}

glm::ivec2 OptionsUI::wantedWindowSize() {
    return defaultWindowSize;
}





OptionsFile::Profile OptionsUI::getActiveProfile() {

    if (optionsFile.profiles.size() == 0) {
        OptionsFile::Profile p;
        return p;
    }

    if (optionsFile.activeProfile >= optionsFile.profiles.size()) {
        optionsFile.activeProfile = optionsFile.profiles.size() - 1;
        LOG_WARN("The profile index was invalid, choosing to the last profile");
    }

    return optionsFile.profiles[optionsFile.activeProfile];
}

void OptionsUI::addProfile(const std::string& name) {
    LOG_INFO("Adding a new profile: '{}'", name);
    OptionsFile::Profile profile;
    profile.name = name;
    optionsFile.activeProfile = optionsFile.profiles.size();
    optionsFile.profiles.push_back(profile);
}

bool OptionsUI::checkSourceFile(const std::string& path) {

    if (path.empty())
        return false;

    if (!Battery::FileExists(path)) {
        return false;
    }

    return true;
}

bool OptionsUI::checkTargetFile(const std::string& path) {

    if (path.empty())
        return false;

    if (Battery::DirectoryExists(path)) {
        return false;
    }

    return true;
}

std::string OptionsUI::openFileDialog() {
    nfdchar_t* outPath = NULL;
    nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outPath);

    std::string path = "";

    if (result == NFD_OKAY) {
        path = outPath;
        free(outPath);
    }

    return path;
}

std::string OptionsUI::saveFileDialog() {
    nfdchar_t* outPath = NULL;
    nfdresult_t result = NFD_SaveDialog("h", NULL, &outPath);

    std::string path = "";

    if (result == NFD_OKAY) {
        path = outPath;
        free(outPath);
    }

    return path;
}


void OptionsUI::OnUpdate() {
    /*if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        auto& window = Battery::GetApp().window;

        float minx = window.getPosition().x;
        float maxx = minx + window.getSize().x;
        float miny = window.getPosition().y;
        float maxy = miny + window.getSize().y;

        auto pos = sf::Mouse::getPosition();
        bool insideWindow = (pos.x > minx && pos.x < maxx) && (pos.y > miny && pos.y < maxy);
        //LOG_INFO("[OptionsUI] Click outside of Window detected, requesting close");
        //shouldBeClosed = true;
    }*/

    if (!Battery::GetApp().window.hasFocus()) {
        noFocusCount++;
    }
    else {
        noFocusCount = 0;
    }

    if (noFocusCount > 5) {
        LOG_INFO("[OptionsUI] Window has no focus, but no event was captured. Anyways, requesting close");
        shouldBeClosed = true;
    }

    profilesDropdown.name = "##Profiles";
    profilesDropdown.items.clear();
    for (auto profile : optionsFile.profiles) {
        profilesDropdown.items.push_back(profile.name);
    }


    if (optionsFile.profiles.size() > 0) {

        if (optionsFile.activeProfile >= optionsFile.profiles.size()) {
            optionsFile.activeProfile = optionsFile.profiles.size() - 1;
            LOG_WARN("The profile index was invalid, choosing to the last profile");
        }

        auto& profile = optionsFile.profiles[optionsFile.activeProfile];

        if (chooseInput) {
            chooseInput = false;
            ignoreFocusLoss = true;
            profile.sourcePath = openFileDialog();
        }
        if (chooseOutput) {
            chooseOutput = false;
            ignoreFocusLoss = true;
            profile.targetPath = saveFileDialog();
        }

        if (sourceFileChecked != profile.sourcePath) {
            sourceFileValid = checkSourceFile(profile.sourcePath);
            sourceFileChecked = profile.sourcePath;
        }
        if (targetFileChecked != profile.targetPath) {
            targetFileValid = checkTargetFile(profile.targetPath);
            targetFileChecked = profile.targetPath;
        }
    }
    else {
        sourceFileValid = false;
        targetFileValid = false;
    }
}

void OptionsUI::OnRender() {
    auto& window = Battery::GetApp().window;

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(7, 7));
    
    ImGui::PushFont(Fonts::robotoOptions);

    ImGui::Text("Arduino MKR Vidor 4000:\nFPGA Bitstream conversion utility");
    ImGui::Separator();
    ImVec2 pos = ImGui::GetCursorPos();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 50);
    ImGui::SetCursorPosY(10);
    ImGui::PushFont(Fonts::fontAwesomeOptionsButtons);
    if (ImGui::Button("\xef\x83\x80")) {                        // Feedback icon
        ImGui::OpenPopup("FeedbackPopup");
    }
    ImGui::PopFont();
    ImGui::HelperPopup("Instant feedback", Fonts::robotoOptionsPopups);
    ImGui::SetCursorPos(pos);


    bool feedback = false;
    bool bugreport = false;
    bool like = false;
    bool dislike = false;
    bool sent = false;
    bool notsent = false;

    ImGui::PushFont(Fonts::robotoOptionsPopups);
    if (ImGui::BeginPopup("FeedbackPopup")) {

        ImGui::PushFont(Fonts::fontAwesomeOptions);
        ImGui::Text("\xef\x83\x80");
        ImGui::SameLine();
        ImGui::PopFont();
        if (ImGui::Button("Feedback")) {
            feedback = true;
            memset(feedbackTextInputBuffer, 0, sizeof(feedbackTextInputBuffer));
        }

        ImGui::PushFont(Fonts::fontAwesomeOptions);
        ImGui::Text("\xef\x86\x88");
        ImGui::SameLine();
        ImGui::PopFont();
        if (ImGui::Button("Bug report")) {
            bugreport = true;
            memset(feedbackTextInputBuffer, 0, sizeof(feedbackTextInputBuffer));
        }

        ImGui::PushFont(Fonts::fontAwesomeOptions);
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "\xef\x85\xa4");
        ImGui::SameLine();
        ImGui::PopFont();
        if (ImGui::Button("I like this app")) {
            like = true;
        }

        ImGui::PushFont(Fonts::fontAwesomeOptions);
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "\xef\x85\xa5");
        ImGui::SameLine();
        ImGui::PopFont();
        if (ImGui::Button("I don't like this app")) {
            dislike = true;
        }

        ImGui::PushFont(Fonts::fontAwesomeOptions);
        ImGui::Text("\xef\x82\x9b");
        ImGui::SameLine();
        ImGui::PopFont();
        if (ImGui::Button("GitHub")) {
            Battery::ExecuteShellCommand("start " + RES->githubIssueUrl);
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
    ImGui::PopFont();

    if (feedback) ImGui::OpenPopup("FeedbackInput");
    if (ImGui::BeginPopup("FeedbackInput", ImGuiWindowFlags_AlwaysAutoResize)) {

        ImGui::PushFont(Fonts::robotoSignature);
        ImGui::InputTextMultiline("##Feedback", feedbackTextInputBuffer, sizeof(feedbackTextInputBuffer), ImVec2(260, 160));
        ImGui::PopFont();

        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("OK")) {
            std::string msg = feedbackTextInputBuffer;
            if (!msg.empty()) {
                if (sendFeedback(msg)) sent = true;
                else notsent = true;
            }
            else {
                LOG_WARN("Text was empty, no feedback sent");
            }

            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    if (bugreport) ImGui::OpenPopup("BugReport");
    if (ImGui::BeginPopup("BugReport", ImGuiWindowFlags_AlwaysAutoResize)) {

        ImGui::PushFont(Fonts::robotoSignature);
        ImGui::InputTextMultiline("##Feedback", feedbackTextInputBuffer, sizeof(feedbackTextInputBuffer), ImVec2(260, 160));
        ImGui::PopFont();

        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("OK")) {
            std::string msg = feedbackTextInputBuffer;
            if (!msg.empty()) {
                if (sendBugreport(msg)) sent = true;
                else notsent = true;
            }
            else {
                LOG_WARN("Text was empty, no bugreport sent");
            }

            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    if (like) {
        if (sendLike()) sent = true;
        else notsent = true;
    }

    if (dislike) {
        if (sendDislike()) sent = true;
        else notsent = true;
    }

    if (sent) ImGui::OpenPopup("FeedbackSent");
    if (ImGui::BeginPopup("FeedbackSent")) {
        ImGui::Text("Thank you for your feedback! :)");
        if (ImGui::Button("OK")) ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }

    if (notsent) ImGui::OpenPopup("FeedbackNotSent");
    if (ImGui::BeginPopup("FeedbackNotSent")) {
        ImGui::Text("Your feedback could not be sent :(");
        if (ImGui::Button("OK")) ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }







    ImGui::Text("Profile");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(160);
    profilesDropdown.Draw(optionsFile.activeProfile);
    ImGui::SameLine();

    if (ImGui::Button("New")) {
        memset(textInputBuffer, 0, sizeof(textInputBuffer));
        ImGui::OpenPopup("New Profile");
    }

    ImGui::SameLine();
    if (ImGui::Button("Delete")) {
        if (optionsFile.profiles.size() != 0) {
            ImGui::OpenPopup("Delete Profile");
        }
    }

    ImGui::PushFont(Fonts::robotoOptionsPopups);
    if (ImGui::BeginPopup("New Profile")) {

        if (ImGui::IsWindowFocused() && !ImGui::IsAnyItemActive())
            ImGui::SetKeyboardFocusHere();

        ImGui::SetNextItemWidth(200);
        bool ok = ImGui::InputText("##Name", textInputBuffer, sizeof(textInputBuffer), ImGuiInputTextFlags_EnterReturnsTrue);
        
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }
            
        ImGui::SameLine();

        if (ImGui::Button("OK") || ok) {
            std::string name = textInputBuffer;
            if (!name.empty()) {
                addProfile(name);
            }
            else {
                LOG_WARN("Name was empty, no profile added");
            }

            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    if (ImGui::BeginPopupModal("Delete Profile", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {

        auto profile = getActiveProfile();
        ImGui::Text("Are you sure you want to\ndelete profile '%s'", profile.name.c_str());

        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("OK")) {
            optionsFile.profiles.erase(optionsFile.profiles.begin() + optionsFile.activeProfile);
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
    ImGui::PopFont();






    auto profile = getActiveProfile();

    ImGui::Text("Input file:    ");
    ImGui::SameLine();

    ImGui::Text(shortenFilename(profile.sourcePath).c_str());
    ImGui::SameLine(ImGui::GetWindowWidth() - 68);

    ImGui::PushFont(Fonts::fontAwesomeOptions);
    if (sourceFileValid)
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "\xef\x80\x8c");	// Green check mark
    else
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "\xef\x80\x8d");	// Red cross
    ImGui::SameLine();

    if (ImGui::Button("\xef\x81\xbc##input")) {
        chooseInput = true;
    }
    ImGui::HelperPopup("Choose the output of your Intel Quartus\n"
                       "project. You should find it under\n"
                       "output_files/MKRVIDOR4000.ttf or similar.", Fonts::robotoOptionsPopups);
    ImGui::PopFont();




    ImGui::Text("Output file: ");
    ImGui::SameLine();

    ImGui::Text(shortenFilename(profile.targetPath).c_str());
    ImGui::SameLine(ImGui::GetWindowWidth() - 68);

    ImGui::PushFont(Fonts::fontAwesomeOptions);
    if (targetFileValid)
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "\xef\x80\x8c");	// Green check mark
    else
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "\xef\x80\x8d");	// Red cross
    ImGui::SameLine();

    if (ImGui::Button("\xef\x81\xbc##output")) {
        chooseOutput = true;
    }
    ImGui::HelperPopup("Choose the future filename it will be exported\n"
                       "to, in your Arduino project. You could call it\n"
                       "FPGA_Bitstream.h or similar.", Fonts::robotoOptionsPopups);
    ImGui::PopFont();


    ImGui::Checkbox("Autostart", &optionsFile.autostart);
    ImGui::HelperPopup("Automatically start when your PC boots up", Fonts::robotoOptionsPopups);

    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 50);
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 50);
    ImGui::PushFont(Fonts::fontAwesomeOptionsButtons);
    if (ImGui::Button("\xef\x80\x91")) {                        // Power off icon
        ImGui::OpenPopup("Quit ByteReverser##Popup");
    }
    ImGui::HelperPopup("Quit the ByteReverser background service", Fonts::robotoOptionsPopups);
    ImGui::PopFont();

    ImGui::PushFont(Fonts::robotoOptionsPopups);
    if (ImGui::BeginPopupModal("Quit ByteReverser##Popup", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Do you really want to quit?");

        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("OK")) {
            Battery::GetApp().CloseApplication();
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
    ImGui::PopFont();

    ImGui::SetCursorPosX(15);
    ImGui::SetCursorPosY(window.getSize().y - 30);

    ImGui::PushFont(Fonts::robotoSignature);
    ImGui::Text("Made with");
    ImGui::SameLine();
    ImGui::PushFont(Fonts::fontAwesomeSignature);
    ImGui::TextColored({ 1, 0, 0, 1 }, "\xef\x80\x84");
    ImGui::SameLine();
    ImGui::PopFont();
    ImGui::Text("by HerrNamenlos123");
    ImGui::PopFont();

    ImGui::PopFont();

    ImGui::PopStyleVar();
}
