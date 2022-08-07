
#include "pch.h"
#include "OptionsUI.h"
#include "GlobalResources.h"
#include "TelegramBot.h"

#include <banana/api.hpp>
#include <banana/agent/default.hpp>

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

void registerNewInstance() {
    OptionsFile::loadOptions();

    LOG_INFO("Install is {}", optionsFile.installRegistered ? "registered" : "unregistered");
    if (!optionsFile.installRegistered) {

        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string date = std::ctime(&now);
        date.pop_back();

        auto when = std::time(nullptr);
        auto const tm = *std::localtime(&when);
        std::ostringstream os;
        os << std::put_time(&tm, "%z");
        std::string timezone = os.str();

        std::string message = "[new install]: " + date + " GMT" + timezone;

        if (sendTelegramFeedbackMessage(message)) {
            optionsFile.installRegistered = true;
            OptionsFile::writeOptions();
        }
    }
}
