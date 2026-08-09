TARGET = app
SRC    = app.c $(wildcard sketches/*.c)

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    # ---- macOS ----
    CC      = clang
    CFLAGS  = -Iinclude -mmacosx-version-min=15.0
    LIBS    = lib/libraylib.a
    LDFLAGS = -framework CoreVideo \
              -framework IOKit \
              -framework Cocoa \
              -framework GLUT \
              -framework OpenGL
else
    # ---- Linux (DRM/GBM) ----
    CC      = gcc
    CFLAGS  = -Iinclude -Wall -std=c99 -D_DEFAULT_SOURCE -DPLATFORM_DRM
    LIBS    =
    LDFLAGS = -lraylib -lGLESv2 -lEGL -lgbm -ldrm -lpthread -lrt -lm -ldl -latomic \
              -I/usr/include/libdrm
endif

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LIBS) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean