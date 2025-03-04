#!/bin/sh
flags="-Wall -Wextra"
links="-lwayland-client -lvulkan"

# Generate Wayland Protocol Codes
if [ ! -d vendor/wayland ]
then
    mkdir -p vendor/wayland
fi

if [ ! -e vendor/wayland/wl_client.h ]
then
    wayland-scanner client-header < /usr/share/wayland/wayland.xml > vendor/wayland/wl_client.h
fi

if [ ! -e vendor/wayland/wl_client.c ]
then
    wayland-scanner private-code < /usr/share/wayland/wayland.xml > vendor/wayland/wl_client.c
fi

if [ ! -e vendor/wayland/xdg_shell.h ]
then
    wayland-scanner client-header < /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml > vendor/wayland/xdg_shell.h
fi

if [ ! -e vendor/wayland/xdg_shell.c ]
then
    wayland-scanner private-code < /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml > vendor/wayland/xdg_shell.c
fi

gcc -o skeel $(find -type f -name "src/*.c") $links $flags -I./src/ -I./vendor/
