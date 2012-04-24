#!/bin/sh

./configure --disable-shared --enable-static --disable-indevs --disable-outdevs \
\
--disable-protocols \
--disable-protocol=http --disable-protocol=tcp --disable-protocol=udp --disable-protocol=rtp \
--enable-protocol=file \
\
--disable-bsfs \
--disable-filters \
\
--disable-encoders \
--enable-libvorbis --enable-encoder=libvorbis \
--enable-libmp3lame --enable-encoder=libmp3lame \
\
--disable-muxers \
--enable-muxer=mp3 \
--enable-muxer=ogg \
\
--disable-decoders \
--enable-decoder=aac \
--enable-decoder=aac_latm \
--enable-decoder=flac \
--enable-decoder=mp3 \
--enable-decoder=mp3adu \
--enable-decoder=mp3adufloat \
--enable-decoder=mp3float \
--enable-decoder=mp3on4 \
--enable-decoder=mp3on4float \
--enable-decoder=pcm_s16be \
--enable-decoder=pcm_s16le \
--enable-decoder=pcm_u16be \
--enable-decoder=pcm_u16le \
--enable-decoder=vorbis \
--enable-decoder=wmapro \
--enable-decoder=wmav1 \
--enable-decoder=wmav2 \
\
--disable-demuxers \
--enable-demuxer=aac \
--enable-demuxer=aiff \
--enable-demuxer=alaw \
--enable-demuxer=au \
--enable-demuxer=avi \
--enable-demuxer=flac \
--enable-demuxer=flv \
--enable-demuxer=mp2 \
--enable-demuxer=mp3 \
--enable-demuxer=mp4 \
--enable-demuxer=mpeg \
--enable-demuxer=ogg \
--enable-demuxer=s16be \
--enable-demuxer=s16le \
--enable-demuxer=u16be \
--enable-demuxer=u16le \
--enable-demuxer=wav \
\
--disable-parsers \
--enable-parser=aac \
--enable-parser=aac_latm \
--enable-parser=flac \
--enable-parser=mpegaudio \

