#!/bin/sh

./configure --disable-shared --enable-static --disable-indevs --disable-outdevs \
\
--disable-protocols \
--disable-protocol=http --disable-protocol=tcp --disable-protocol=udp --disable-protocol=rtp \
--enable-protocol=file \
\
--disable-encoders \
--enable-libvorbis --enable-encoder=libvorbis \
--enable-libmp3lame --enable-encoder=libmp3lame
