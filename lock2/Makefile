.SUFFIXES: .c .o

CC=gcc

SRCDIR=src/
INC=include/
LIBS=lib/

# SRCS:=$(wildcard src/*.c)
# OBJS:=$(SRCS:.c=.o)

# main source file
TARGET_SRC:=$(SRCDIR)test2.c
TARGET_OBJ:=$(SRCDIR)test2.o

# Include more files if you write another source file.
SRCS_FOR_LIB:=$(SRCDIR)hybrid_lock.c
OBJS_FOR_LIB:=$(SRCS_FOR_LIB:.c=.o)

CFLAGS+= -g -fPIC -I $(INC)

TARGET=main

all: $(TARGET)

$(TARGET): $(TARGET_OBJ)
	$(CC) $(CFLAGS) -o $(OBJS_FOR_LIB) -c $(SRCS_FOR_LIB)
	make static_library
	$(CC) $(CFLAGS) -o $@ $^ -L $(LIBS) -lhybrid_lock -lpthread

clean:
	rm $(TARGET) $(TARGET_OBJ) $(OBJS_FOR_LIB) $(LIBS)*

library:
	gcc -shared -Wl,-soname,libhybrid_lock.so -o $(LIBS)libhybrid_lock.so $(OBJS_FOR_LIB)

static_library:
	ar cr $(LIBS)libhybrid_lock.a $(OBJS_FOR_LIB)
