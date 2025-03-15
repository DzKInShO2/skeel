#include "defines.h"

#include <string.h>
#include <stdio.h>

#include "wayland/wl_client.h"
#include "wayland/xdg_shell.h"

typedef struct wl_context wl_context;
struct wl_context {
    struct wl_display *display;
    struct wl_registry *registry;
    struct wl_compositor *compositor;
    struct wl_seat *seat;
    struct xdg_wm_base *xdg_wm_base;

    struct wl_surface *surface;
    struct xdg_surface *xdg_surface;
    struct xdg_toplevel *xdg_toplevel;
};

static void wl_registry_global(void *data, struct wl_registry *registry,
                        uint32 name, const char *interface, uint32 version)
{
    struct wl_context *ctx = (struct wl_context *)data;
    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        ctx->compositor = wl_registry_bind(registry, name,
                                           &wl_compositor_interface, version);
    } else if (strcmp(interface, wl_seat_interface.name) == 0) {
        ctx->seat = wl_registry_bind(registry, name,
                                     &wl_seat_interface, version);
    } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        ctx->xdg_wm_base = wl_registry_bind(registry, name,
                                         &xdg_wm_base_interface, version);
    }
}

static void wl_registry_global_remove(void *data, struct wl_registry *registry,
                               uint32 name)
{
    (void)data;
    (void)registry;
    (void)name;
}

static void xdg_wm_base_ping(void *data, struct xdg_wm_base *wm_base,
                             uint32 serial)
{
    (void)data;
    xdg_wm_base_pong(wm_base, serial);
}

static void xdg_surface_configure(void *data, struct xdg_surface *xdg_surface,
                                  uint32 serial)
{
    (void)data;
    xdg_surface_ack_configure(xdg_surface, serial);
}

int main(void)
{
    struct wl_context ctx = { 0 };
    ctx.display = wl_display_connect(0);
    ctx.registry = wl_display_get_registry(ctx.display);

    struct wl_registry_listener registry_listener = {
        .global = wl_registry_global,
        .global_remove = wl_registry_global_remove
    };

    wl_registry_add_listener(ctx.registry, &registry_listener, (void *)&ctx);
    wl_display_roundtrip(ctx.display);

    struct xdg_wm_base_listener xdg_wm_base_listener = {
        .ping = xdg_wm_base_ping
    };

    xdg_wm_base_add_listener(ctx.xdg_wm_base, &xdg_wm_base_listener, null);

    ctx.surface = wl_compositor_create_surface(ctx.compositor);
    ctx.xdg_surface = xdg_wm_base_get_xdg_surface(ctx.xdg_wm_base, ctx.surface);
    ctx.xdg_toplevel = xdg_surface_get_toplevel(ctx.xdg_surface);

    static struct xdg_surface_listener xdg_surface_listener = {
        .configure = xdg_surface_configure
    };

    xdg_surface_add_listener(ctx.xdg_surface, &xdg_surface_listener, null);

    xdg_surface_set_window_geometry(ctx.xdg_surface, 0, 0, 1920, 1080);
    xdg_toplevel_set_title(ctx.xdg_toplevel, "Skeel");

    wl_surface_commit(ctx.surface);
    while (wl_display_dispatch(ctx.display) != -1) {
    }
    
    xdg_toplevel_destroy(ctx.xdg_toplevel);
    xdg_surface_destroy(ctx.xdg_surface);
    wl_surface_destroy(ctx.surface);
    wl_display_disconnect(ctx.display);
}
