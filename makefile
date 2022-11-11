name := Neutrons
G4TARGET := $(name)
G4EXLIB := true
G4DEBUG=1
DEBUG ?=-ggdb
CPPFLAGS+=-Wno-overloaded-virtual -Wno-unused

.PHONY: all 
all: lib bin

CPPFLAGS+=-Wno-overloaded-virtual -Wno-unused

include $(G4INSTALL)/config/binmake.gmk

ROOTINC := -I$(shell root-config --incdir)
INCFLAGS += $(ROOTINC)
ROOTLIBS := $(shell root-config --nonew --libs)
LDLIBS += $(ROOTLIBS)