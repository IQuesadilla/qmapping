CC := gcc
TARGET := qmapping
SRCS := AppEvent.c AppInit.c AppIterate.c
INCS := Global.h
LIBS := -lSDL3 -lGL -lm

.PHONY : all run
all: ${TARGET}
run: ${TARGET}
	nixGL ./${TARGET}

${TARGET}: ${SRCS} ${INCS}
	$(CC) -o ${TARGET} ${SRCS} ${LIBS}
