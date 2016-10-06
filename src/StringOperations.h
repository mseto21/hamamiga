#pragma once
#include "Types.h"

namespace StringOperations {
	void str128copy(String128* destination, const char* path) {
		memcpy(destination, path, sizeof(String128));
	}

	void str256copy(String256* destination, const char* path) {
		memcpy(destination, path, sizeof(String128));
	}
}