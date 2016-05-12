# Makefile to build the C version of jl_lib.
DEPS_VER_SDL = SDL2-2.0.4
DEPS_VER_SDL_IMAGE = SDL2_image-2.0.1
DEPS_VER_SDL_MIXER = SDL2_mixer-2.0.1
DEPS_VER_SDL_NET = SDL2_net-2.0.1
DEPS_VER_ZIP = libzip-1.1.2

ifneq ("$(shell uname | grep Linux)", "")
 ifneq ("$(shell uname -m | grep arm)", "")
  include compile-scripts/rpi.mk
 else
  include compile-scripts/linux.mk
 endif
else
 $(error "Platform is not supported")
endif
#TODO: Darwin is mac OS for uname

HEADER = -Isrc/lib/ -Isrc/lib/include/ -I/opt/vc/include/ \
	$(addprefix -I, $(shell find src/C/ -type d ))
CC = gcc

CLUMP_SRC = src/lib/clump
SRC = src/C
BUILD = build/obj
# 
MODULES = $(subst .c,, $(shell basename -a \
	$(shell find $(SRC)/ -type f -name '*.c') \
	$(shell find $(CLUMP_SRC)/ -type f -name '*.c') \
))
HEADERS = $(shell find $(SRC)/ -type f -name '*.h')
# Special MAKE variable - do not rename.
VPATH = \
	$(shell find $(SRC)/ -type d) \
	$(shell find $(CLUMP_SRC)/ -type d)
#
OBJS = $(addprefix $(BUILD)/, $(addsuffix .o,$(MODULES)))
SHARED = $(BUILD)/jl.so
STATIC = $(BUILD)/jl.a

# Locations
LIBZIP = build/android/jni/src/lib/libzip/
SRC_NDK = android-ndk-r10e

# The Set-Up Options.
init-all: init-build deps-all
init-most: init-build deps-most

# The Clean Options.
clean-all: clean-build clean-deps
clean-build:
	# Empty directory: build/obj/
	printf "[COMP] Cleaning up....\n"
	rm -f -r build/obj/
	mkdir -p build/obj/
	rm -f build/*.o
	printf "[COMP] Done!\n"
clean-build-all:
	rm -r build/
clean-build-andr:
	rm -r build/android/
clean-deps:
	rm -r deps/

# The Build Options.
build-android-jl_lib:
	# Copy android build files into android build project.
	cp -u -t build/android/jni/ android-src/*.mk
	cp -u -t build/android/ android-src/*.properties
	cp -u -t build/android/src/org/libsdl/app/ android-src/SDL*.java 
	cp -u --recursive -t build/android/jni/src/ src/*
build-android: build-android-jl_lib
	# Copy SDL2 into android build project
	cp -u android-src/SDL_android_main.c build/android/jni/SDL_android_main.c
	cp -u --recursive -t build/android/jni/src/lib/sdl/\
	 deps/$(DEPS_VER_SDL)/src/*
	cp -u build/android/jni/src/lib/include/SDL_config_android.h\
	 build/android/jni/src/lib/include/SDL_config.h
	# Copy Libzip into android project
	cp -u deps/$(DEPS_VER_ZIP)/config.h build/android/jni/src/lib/libzip/
	cp -u --recursive -t build/android/jni/src/lib/libzip/\
	 deps/$(DEPS_VER_ZIP)/lib/*.c
	cp -u --recursive -t build/android/jni/src/lib/libzip/\
	 deps/$(DEPS_VER_ZIP)/lib/*.h
	rm $(LIBZIP)zipwin32.h $(LIBZIP)*win32*.c $(LIBZIP)zip_add.c \
		$(LIBZIP)zip_add_dir.c $(LIBZIP)zip_error_get.c \
		$(LIBZIP)zip_error_get_sys_type.c $(LIBZIP)zip_error_to_str.c \
		$(LIBZIP)zip_file_error_get.c $(LIBZIP)zip_get_file_comment.c \
		$(LIBZIP)zip_get_num_files.c $(LIBZIP)zip_rename.c \
		$(LIBZIP)zip_replace.c $(LIBZIP)zip_set_file_comment.c
	# Copy SDL2_mixer into the android project.
	cp -u --recursive -t build/android/jni/src/lib/sdl-mixer/external/\
	 deps/$(DEPS_VER_SDL_MIXER)/external/*
	cp -u --recursive -t build/android/jni/src/lib/sdl-mixer/\
	 deps/$(DEPS_VER_SDL_MIXER)/*.c
	cp -u --recursive -t build/android/jni/src/lib/sdl-mixer/\
	 deps/$(DEPS_VER_SDL_MIXER)/*.h
	rm build/android/jni/src/lib/sdl-mixer/play*.c
	# Copy SDL2_image into the android project.
	cp -u --recursive -t build/android/jni/src/lib/sdl-image/\
	 deps/$(DEPS_VER_SDL_IMAGE)/*
	cp -u android-src/jconfig.h\
	 build/android/jni/src/lib/sdl-image/external/jpeg-9/jconfig.h
	# Copy SDL2_net into the android project.
	cp -u --recursive -t build/android/jni/src/lib/sdl-net/\
	 deps/$(DEPS_VER_SDL_NET)/*.c
	cp -u --recursive -t build/android/jni/src/lib/sdl-net/\
	 deps/$(DEPS_VER_SDL_NET)/*.h

$(BUILD)/%.o: %.c $(HEADERS)
	printf "[COMP] compiling $<....\n"
	$(CC) $(CFLAGS) -o $@ -c $<

build/jl.o: $(BUILD) $(OBJS)
	printf "[COMP] compiling singular jl_lib object file....\n"
	ar csr build/jl.o build/obj/*.o build/deps/*.o

-test: 
	$(eval CFLAGS=-Wall -g $(HEADER) $(PLATFORM_CFLAGS))

-release:
	$(eval CFLAGS=-Wall -O3 $(HEADER) $(PLATFORM_CFLAGS))

build-notify:
	echo Modules: $(MODULES)
	echo Headers: $(HEADERS)
	echo Folders: $(VPATH)
	printf "[COMP] Building jl_lib for target=$(PLATFORM)\n"

# Build modules.
test: -test build-notify build/jl.o
	# Make "jl.o"
	printf "[COMP] done!\n"

release: -release build-notify build/jl.o
	# Make "jl.o"
	printf "[COMP] done!\n"

# Lower Level
deps-all: deps-most download-ndk
deps-most: deps/ src/lib/include/ deps-sdl deps-libzip deps-sdl-net\
	deps-sdl-image deps-sdl-mixer

init-build:
	mkdir -p build/deps/
	mkdir -p build/obj/

deps/:
	mkdir -p deps/

src/lib/include/:
	mkdir -p src/lib/include/

-download-sdl:
	cd deps/ && \
	wget https://www.libsdl.org/release/$(DEPS_VER_SDL).zip && \
	unzip $(DEPS_VER_SDL).zip && \
	rm $(DEPS_VER_SDL).zip

download-ems:
	cd deps/&& \
	wget https://s3.amazonaws.com/mozilla-games/emscripten/releases/emsdk-portable.tar.gz && \
	tar -xzf emsdk-portable.tar.gz && \
	rm emsdk-portable.tar.gz

download-ndk:
	cd deps/ && \
	wget http://dl.google.com/android/ndk/$SRC_NDK-linux-x86_64.bin\
	 --progress=bar && \
	chmod a+x $SRC_NDK-linux-x86_64.bin && \
	./$SRC_NDK-linux-x86_64.bin && \
	rm $SRC_NDK-linux-x86_64.bin

download-sdk:
	cd deps/ && \
	wget https://dl.google.com/android/android-sdk_r24.4.1-linux.tgz && \
	tar -zxvf android-sdk_r24.4.1-linux.tgz && \
	rm android-sdk_r24.4.1-linux.tgz && \
	cd android-sdk-linux/ && \
	./tools/android

# Low Level / Build
build-emscripten:
	printf "[COMP] comiling emscripten...\n" && \
	cd deps/emsdk_portable/ && \
	./emsdk update && \
	./emsdk install latest && \
	./emsdk activate latest
deps-libzip:
	printf "[COMP] downloading libzip...\n" && \
	cd deps/ && \
	wget -4 http://www.nih.at/libzip/$(DEPS_VER_ZIP).tar.gz && \
	tar -xzf $(DEPS_VER_ZIP).tar.gz && \
	rm $(DEPS_VER_ZIP).tar.gz && \
	printf "[COMP] compiling libzip...\n" && \
	cd $(DEPS_VER_ZIP)/ && \
	sh configure && \
	make && \
	ld -r lib/*.o -o ../../build/deps/lib_zip.o && \
	cp lib/*.h ../../src/lib/include/ && \
	printf "[COMP] done!\n"
deps-sdl-image:
	printf "[COMP] Downloading SDL_Image....\n" && \
	cd deps/ && \
	wget\
	 https://www.libsdl.org/projects/SDL_image/release/$(DEPS_VER_SDL_IMAGE).zip\
	 && \
	unzip $(DEPS_VER_SDL_IMAGE).zip && \
	rm $(DEPS_VER_SDL_IMAGE).zip && \
	export PATH=$$PATH:`pwd`/$(DEPS_VER_SDL)/usr_local/bin/ && \
	printf "[COMP] compiling SDL_image...\n" && \
	cd $(DEPS_VER_SDL_IMAGE)/ && \
#	autoreconf -vfi && \
	sh configure && \
	make && \
	ld -r .libs/*.o -o ../../build/deps/lib_SDL_image.o && \
#	printf "[COMP] compiling libjpeg...\n" && \
#	cp external/jpeg-9/ ../../src/lib/ -r && \
#	rm -f ../../src/lib/jpeg-9/jmemmac.c ../../src/lib/jpeg-9/jmem-android.c \
#		../../src/lib/jpeg-9/jmemdos.c ../../src/lib/jpeg-9/example.c \
#		../../src/lib/jpeg-9/jmemname.c && \
	cp -t ../../src/lib/include/ SDL_image.h && \
#external/jpeg-9/jpeglib.h \
#external/jpeg-9/jconfig.h external/jpeg-9/jmorecfg.h && \
	printf "[COMP] done!\n"
deps-sdl-net:
	printf "[COMP] downloading SDL_net...\n" && \
	cd deps/ && \
	wget https://www.libsdl.org/projects/SDL_net/release/$(DEPS_VER_SDL_NET).zip\
	 && \
	unzip $(DEPS_VER_SDL_NET).zip && \
	rm $(DEPS_VER_SDL_NET).zip && \
	printf "[COMP] compiling SDL_net...\n" && \
	cd $(DEPS_VER_SDL_NET)/ && \
	export SDL2_CONFIG=`pwd`/../$(DEPS_VER_SDL)/usr_local/bin/sdl2-config && \
	sh configure && \
	make && \
	ar csr ../../build/deps/lib_SDL_net.o .libs/*.o && \
	cp SDL_net.h ../../src/lib/include/ && \
	printf "[COMP] done!\n"

deps-sdl-mixer:
	printf "[COMP] downloading SDL_mixer...\n" && \
	cd deps/ && \
	wget\
	 https://www.libsdl.org/projects/SDL_mixer/release/$(DEPS_VER_SDL_MIXER).zip\
	 && \
	unzip $(DEPS_VER_SDL_MIXER).zip && \
	rm $(DEPS_VER_SDL_MIXER).zip && \
	export PATH=$$PATH:`pwd`/$(DEPS_VER_SDL)/usr_local/bin/ && \
	printf "[COMP] compiling SDL_mixer...\n" && \
	cd $(DEPS_VER_SDL_MIXER)/ && \
	sh configure && \
	make && \
	rm -f build/playmus.o build/playwave.o && \
	printf "[COMP] Linking...\n" && \
	ld -r build/*.o -o ../../build/deps/lib_SDL_mixer.o && \
	cp SDL_mixer.h ../../src/lib/include/SDL_mixer.h && \
	printf "[COMP] done!\n"

build-clump:
	printf "[COMP] compiling clump...\n"
	gcc src/lib/clump/bitarray.c -c -o build/obj/clump_bitarray.o
	gcc src/lib/clump/clump.c -c -o build/obj/clump_clump.o
	gcc src/lib/clump/hash.c -c -o build/obj/clump_hash.o
	gcc src/lib/clump/hcodec.c -c -o build/obj/clump_hcodec.o
	gcc src/lib/clump/list.c -c -o build/obj/clump_list.o
	gcc src/lib/clump/pool.c -c -o build/obj/clump_pool.o
	gcc src/lib/clump/tree.c -c -o build/obj/clump_tree.o
	ar csr build/deps/lib_clump.o build/obj/clump_*.o
	printf "[COMP] done!\n"	

################################################################################
