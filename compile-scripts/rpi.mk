PLATFORM = Raspberry Pi
PLATFORM_CFLAGS = -DJL_PLAT_RPI

deps-sdl: -download-sdl
	printf "[COMP] compiling SDL for $(PLATFORM)...\n" && \
	printf "[COMP] compiling SDL Dependencies....\n" && \
	sudo apt-get update && \
	sudo apt-get install -y libudev-dev libevdev-dev\
		libasound2-dev \
		libdbus-1-dev libpng12-dev \
		libtiff5-dev libwebp-dev libvorbis-dev libflac-dev \
		--fix-missing
	printf "[COMP] compiling SDL...\n" && \
	cd deps/$(DEPS_VER_SDL)/ && \
	sh configure --prefix=`pwd`/usr_local/ --host=armv7l-raspberry-linux-gnueabihf --disable-pulseaudio --disable-esd --disable-video-mir --disable-video-wayland --disable-video-x11 --disable-video-opengl --enable-libudev --enable-libevdev --enable-input-tslib && \
	make -j 4 && make install && \
	ld -r build/.libs/*.o -o ../../build/deps/lib_SDL.o && \
	cp include/*.h ../../src/lib/include/ && \
	printf "[COMP] done!\n"
