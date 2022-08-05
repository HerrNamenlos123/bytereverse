#pragma once

#include "pch.h"
#include "Fonts.h"

class _BaseUI {
public:
	_BaseUI() {}

	void setupWindow(uint16_t width, uint16_t height, uint8_t alpha = 255);
	void moveWindowDelta(int dx, int dy);

	bool canBeClosed = false;
	bool shouldBeClosed = false;
	glm::ivec2 originalSize;

	virtual void update() = 0;
	virtual void render() = 0;

private:

};
