CC := clang
TARGET := qmapping.exe
SRCS := AppEvent.c AppInit.c AppIterate.c
INCS := Global.h
LIBS := -lSDL3 -lglew32 -lopengl32
WIN := -Wl,/SUBSYSTEM:WINDOWS

.PHONY : all run
all: ${TARGET}
run: ${TARGET}
	nixGL ./${TARGET}

${TARGET}: ${SRCS} ${INCS}
	$(CC) -o ${TARGET} ${SRCS} ${LIBS} -I 'C:\Program Files (x86)\SDL3\include' -L 'C:\Program Files (x86)\SDL3\lib'
