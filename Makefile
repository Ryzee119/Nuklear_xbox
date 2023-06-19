XBE_TITLE = Nuklear
GEN_XISO = $(XBE_TITLE).iso
NXDK_SDL = y

# Add pbgl
PBGL_DIR = $(CURDIR)/pbgl
PBGL_SRCS = $(wildcard $(PBGL_DIR)/src/*.c)
PBGL_FLAGS = -I$(PBGL_DIR)/include -DXBOX

SRCS +=	main.c $(PBGL_SRCS)
CFLAGS += -I$(CURDIR) $(PBGL_FLAGS) -DDEBUG_CONSOLE

include $(NXDK_DIR)/Makefile

print_variables:
	@echo $(CFLAGS)