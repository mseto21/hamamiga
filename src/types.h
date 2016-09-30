#include <cstdint>

typedef uint8_t 	uint8;
typedef uint16_t 	uint16;
typedef uint32_t 	uint32;
typedef uint64_t 	uint64;
typedef char[128]   String128;
typedef char[256]   String256;

static void str128copy(String128* destination, const char* path) {
	memcpy(destination, path, sizeof(String128));
}

static void str256copy(String256* destination, const char* path) {
	memcpy(destination, path, sizeof(String128));
}