
#include "pch.h"
#include "OptionsUI.h"
#include "GlobalResources.h"
#include "TelegramBot.h"
#include "UUID.h"

#include <banana/api.hpp>
#include <banana/agent/default.hpp>

static std::string getTimeAndDateString() {

    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string date = std::ctime(&now);
    date.pop_back();

    auto when = std::time(nullptr);
    auto const tm = *std::localtime(&when);
    std::ostringstream os;
    os << std::put_time(&tm, "%z");
    std::string timezone = os.str();

    return date + " GMT" + timezone;
}

bool sendTelegramFeedbackMessage(const std::string& msg) {
    try {
        LOG_INFO("Sending Telegram feedback message: '{}'", msg);
        banana::agent::default_blocking agent(RES->TelegramApiToken);
        banana::api::send_message(agent, { RES->feedbackChatID, msg });
        return true;
    }
    catch (const std::exception& e) {
        LOG_ERROR("Error while sending telegram feedback: {}", e.what());
        return false;
    }
}

bool sendFeedback(const std::string& msg) {
    std::string message = "Feedback at " + getTimeAndDateString() + "\n";
    message += "Version: v" + RES->versionString + "\n";
    message += "Install UUID: " + optionsFile.installationUUID + "\n";
    message += msg;
    return sendTelegramFeedbackMessage(message);
}

bool sendBugreport(const std::string& msg) {
    std::string message = "Bugreport at " + getTimeAndDateString() + "\n";
    message += "Version: v" + RES->versionString + "\n";
    message += "Install UUID: " + optionsFile.installationUUID + "\n";
    message += msg;
    return sendTelegramFeedbackMessage(message);
}

bool sendLike() {
    std::string message = "Like at " + getTimeAndDateString() + "\n";
    message += "Version: v" + RES->versionString + "\n";
    message += "Install UUID: " + optionsFile.installationUUID;
    return sendTelegramFeedbackMessage(message);
}

bool sendDislike() {
    std::string message = "Dislike at " + getTimeAndDateString() + "\n";
    message += "Version: v" + RES->versionString + "\n";
    message += "Install UUID: " + optionsFile.installationUUID;
    return sendTelegramFeedbackMessage(message);
}

void registerNewInstance() {
    OptionsFile::loadOptions();

    LOG_INFO("Install is {}", optionsFile.installRegistered ? "registered" : "unregistered");
    if (!optionsFile.installRegistered) {

        optionsFile.installationUUID = UUID::generate_uuid();

        std::string message = "New install at " + getTimeAndDateString() + "\n";
        message += "Version: v" + RES->versionString + "\n";
        message += "Install UUID: " + optionsFile.installationUUID;
        if (sendTelegramFeedbackMessage(message)) {
            optionsFile.installRegistered = true;
            OptionsFile::writeOptions();
        }
    }
}
