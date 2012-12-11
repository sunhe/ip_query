#make variants
CC = gcc
CFLAGS = -Wall -g3 
CPPFLAGS = $(addprefix -D, $(DEFINES)) $(addprefix -I, $(INCLUDEPATH))
LDFLAGS = $(LD_OPTIONS) $(addprefix -L, $(LIBPATH)) $^ $(LIBS)
TARGET_ARCH =
C_INCLUDE_PATH =
CPLUS_INCLUDE_PATH =
#user variants
DEFINES =
INCLUDEPATH = inc 
LD_OPTIONS = 
LIBS = -lip_query
LIBPATH = lib
SOURCE = main.c
OBJECTS = $(patsubst %.c, %.o, $(SOURCE))
#OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c)) $(patsubst %.cpp, %.o, $(wildcard *.cpp))
TOPDIR = $(PWD)
SUBDIRS = src
OUT = test_ip_query

export TOPDIR
export C_INCLUDE_PATH
export CPLUS_INCLUDE_PATH

.PHONY: all install clean distclean debug $(SUBDIRS)
all: $(SUBDIRS) $(OUT)
$(SUBDIRS):
	$(MAKE) -C $@
$(OUT): $(OBJECTS)
	$(LINK.o) $(OUTPUT_OPTION) 
#OUTPUT_OPTION = -o $@
#LINK.o = $(CC) $(LDFLAGS) $(TARGET_ARCH)

clean:
	for d in $(SUBDIRS); do $(MAKE) -C  $$d clean; done
	$(RM) *.o
	$(RM) $(OUT)
distclean:
	for d in $(SUBDIRS); do $(MAKE) -C  $$d clean; done
	$(RM) *.o
	$(RM) $(OUT)
	$(RM) cscope*
	$(RM) tags
