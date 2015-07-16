#!/bin/sh

mkdir -p config
mkdir -p m4
autoreconf -v --force --install -I config -I m4
