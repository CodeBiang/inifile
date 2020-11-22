CC = gcc
RM = rm -rf

OBJS := inistr.o inilist.o inimap.o inifile.o

LIB_NAME = inifile
CJSONX_STATIC = lib$(LIB_NAME).a
CJSONX_SHARED = lib$(LIB_NAME).so


CFLAGS = -Wall -Werror -fPIC -std=gnu99

.PHONY: clean static shared demo

all: static shared

static: $(CJSONX_STATIC)

shared: $(CJSONX_SHARED)

demo:
	$(CC) $(CFLAGS) inistr.c inilist.c inimap.c inifile.c initest.c -o demo -I.

$(CJSONX_STATIC): $(OBJS)
	@ $(AR) rcs $@ $^

$(CJSONX_SHARED): $(OBJS)
	@ $(CC) -shared -o $@ $(CFLAGS) $^

$(OBJS): %.o : %.c
	@ $(CC) $(CFLAGS)  -c $< -o $@

clean:
	@ $(RM) *.o *.a *.so *.demo
