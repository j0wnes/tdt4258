#!/bin/sh
set -x

ptxdist clean game
ptxdist compile game
ptxdist targetinstall game
ptxdist image root.romfs
ptxdist test flash-rootfs