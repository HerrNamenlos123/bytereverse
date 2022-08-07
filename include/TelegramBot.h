#pragma once

#include <string>

bool sendTelegramFeedbackMessage(const std::string& msg);
bool sendFeedback(const std::string& msg);
bool sendBugreport(const std::string& msg);
bool sendLike();
bool sendDislike();

void registerNewInstance();
