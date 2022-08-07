#pragma once

#include "cereal.h"

class FileDaemon {
public:
	FileDaemon(std::string filename) : filename(filename) {}
	virtual ~FileDaemon() {}

	template<typename T>
	class Property {
	public:
		Property() {}

		operator T() {
			
		}

	private:
		T value;
	};

private:
	std::string filename;
};
