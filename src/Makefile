#make variants
CC = gcc
CFLAGS = -Wall -g3 -fPIC
CPPFLAGS = $(addprefix -D, $(DEFINES)) $(addprefix -I, $(INCLUDEPATH))
LDFLAGS = $(LD_OPTIONS) $(addprefix -L, $(LIBPATH)) $^ $(LIBS)
TARGET_ARCH =
C_INCLUDE_PATH =
CPLUS_INCLUDE_PATH =
#user variants
DEFINES = IQ_MEMPOOL
INCLUDEPATH = ../inc
LD_OPTIONS = 
LIBS = 
LIBPATH = .
SOURCE = iq_string.c iq_database.c iq_trie.c ip_query.c
OBJECTS = $(patsubst %.c, %.o, $(SOURCE))
#OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c)) $(patsubst %.cpp, %.o, $(wildcard *.cpp))
OUT = ../lib/libip_query.a

export TOPDIR
export C_INCLUDE_PATH
export CPLUS_INCLUDE_PATH

.PHONY: all install clean distclean run debug
all: $(OUT)
$(OUT): $(OBJECTS)
	$(AR) rsv $@ $^
#OUTPUT_OPTION = -o $@
#LINK.o = $(CC) $(LDFLAGS) $(TARGET_ARCH)

clean:
	$(RM) *.o
	$(RM) $(OUT)
distclean:
	$(RM) *.o
	$(RM) $(OUT)
	$(RM) cscope*
	$(RM) tags
run:
	./$(OUT)
debug:
	gdb --args ./$(OUT)
