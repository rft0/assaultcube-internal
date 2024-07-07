CC = clang
CFLAGS = -Wall -std=c99 -m32

UNICODE = -Xclang -DUNICODE -Xclang -D_UNICODE
DLL = -fuse-ld=lld-link -shared

SRCS = $(wildcard src/*.c) $(wildcard src/cheats/*.c) $(wildcard src/thirdparty/*.c)

LIBS = -lshell32 -luser32 -lopengl32 -lgdi32

all: dev

dev:
	$(CC) $(CFLAGS) $(DLL) $(UNICODE) $(SRCS) $(LIBS) -o bin/main.dll

run:
	.\bin\inj_x86.exe