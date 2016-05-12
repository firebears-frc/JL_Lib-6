PLATFORM = Linux

deps-sdl: -download-sdl
	printf "[COMP] compiling SDL...\n" && \
	cd deps/$(DEPS_VER_SDL)/ && \
	sh configure --prefix=`pwd`/usr_local/ && \
	make -j 4 && make install && \
	ld -r build/.libs/*.o -o ../../build/deps/lib_SDL.o && \
	cp include/*.h ../../src/lib/include/ && \
	printf "[COMP] done!\n"
