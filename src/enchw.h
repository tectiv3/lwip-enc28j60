#include <stdint.h>

#if defined(__cplusplus)
	extern "C" {                 // Make sure we have C-declarations in C++ programs
#endif

typedef struct {
} enchw_device_t;

void enchw_setup(enchw_device_t *dev);
void enchw_select(enchw_device_t *dev);
void enchw_unselect(enchw_device_t *dev);
uint8_t enchw_exchangebyte(enchw_device_t *dev, uint8_t byte);

#if defined(__cplusplus)
}
#endif