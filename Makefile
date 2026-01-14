CC      = clang
CFLAGS  = -Iinclude -mmacosx-version-min=15.0
LDFLAGS = -framework CoreVideo \
          -framework IOKit \
          -framework Cocoa \
          -framework GLUT \
          -framework OpenGL

LIBS    = lib/libraylib.a
TARGET  = app
SRC     = app.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LIBS) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)

