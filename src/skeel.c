#include "defines.h"

#include <string.h>

#include "wayland/wl_client.h"

static void wl_registry_global(void *data, struct wl_registry *regitry,
                        uint32 name, const char *interface, uint32 version) {
}

static void wl_registry_global_remove(void *data, struct wl_registry *regitry,
                               uint32 name) {
}

int main(void)
{
    struct wl_display *display = wl_display_connect(0);
    struct wl_registry *registry = wl_display_get_registry(display);

    struct wl_registry_listener registry_listener = {
        .global = wl_registry_global,
        .global_remove = wl_registry_global_remove
    };

    wl_registry_add_listener(registry, &registry_listener, null);

    while (wl_display_dispatch(display) != -1) {
    }

    wl_display_disconnect(display);
}
