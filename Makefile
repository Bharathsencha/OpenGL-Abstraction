# BLZ Graphics & Audio Library Makefile

# Enable multicore compilation
ifeq (,$(filter -j%,$(MAKEFLAGS)))
MAKEFLAGS += -j$(shell nproc)
endif

#CC = clang
CC = gcc
CFLAGS = -std=c11 -O3 -Wall -Wextra -I. -Isrc
LDFLAGS = -Llib -lglfw3 -lGL -lm -lpthread -ldl -lrt -lX11

OBJDIR = obj
BINDIR = bin
SRCDIR = src

DEMO_TARGET = $(BINDIR)/blz_demo
CAR_TARGET = $(BINDIR)/car_demo
MUSIC_TARGET = $(BINDIR)/music_demo

LIB_SRCS = $(SRCDIR)/gl_backend.c $(SRCDIR)/audio_backend.c
LIB_OBJS = $(OBJDIR)/gl_backend.o $(OBJDIR)/audio_backend.o

all: $(DEMO_TARGET) $(CAR_TARGET) $(MUSIC_TARGET)

$(DEMO_TARGET): $(LIB_OBJS) $(OBJDIR)/main.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(CAR_TARGET): $(LIB_OBJS) $(OBJDIR)/car.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(MUSIC_TARGET): $(LIB_OBJS) $(OBJDIR)/music.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

run: $(DEMO_TARGET)
	./$(DEMO_TARGET)

run-car: $(CAR_TARGET)
	./$(CAR_TARGET)

run-music: $(MUSIC_TARGET)
	./$(MUSIC_TARGET)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all run run-car run-music clean
