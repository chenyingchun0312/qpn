##############################################################################
# Product: Makefile for QP-nano, DPP GUI, Win32-QV, MinGW
# Last updated for version 6.3.7
# Last updated on  2018-11-06
#
#                    Q u a n t u m  L e a P s
#                    ------------------------
#                    Modern Embedded Software
#
# Copyright (C) 2005-2018 Quantum Leaps, LLC. All rights reserved.
#
# This program is open source software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Alternatively, this program may be distributed and modified under the
# terms of Quantum Leaps commercial licenses, which expressly supersede
# the GNU General Public License and are specifically designed for
# licensees interested in retaining the proprietary status of their code.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
# Contact information:
# https://www.state-machine.com
# mailto:info@state-machine.com
##############################################################################
#
# examples of invoking this Makefile:
# building configurations: Debug (default), Release, and Spy
# make
# make CONF=rel
#
# cleaning configurations: Debug (default), Release, and Spy
# make clean
# make CONF=rel clean
#
# NOTE:
# To use this Makefile on Windows, you will need the GNU make utility, which
# is included in the QTools collection for Windows, see:
#    http://sourceforge.net/projects/qpc/files/QTools/
#

#-----------------------------------------------------------------------------
# project name
#
PROJECT := game-gui

#-----------------------------------------------------------------------------
# project directories:
#

# list of all source directories used by this project
VPATH := . \
	..

# list of all include directories needed by this project
INCLUDES := -I. \
	-I..

# list of resource include directories needed by this project
RCINCLUDES = \
	-IRes

# location of the QP-nano framework (if not provided in an env. variable)
ifeq ($(QPN),)
QPN := ../../../..
endif

#-----------------------------------------------------------------------------
# project files:
#

# C source files...
C_SRCS := \
	bsp.c \
	main.c \
	mine1.c \
	mine2.c \
	missile.c \
	ship.c \
	tunnel.c

# C++ source files...
CPP_SRCS  :=

# Resource files...
RC_SRCS := \
	game-gui.rc

LIB_DIRS  :=
LIBS      :=

# defines...
# QP_API_VERSION controls the QP API compatibility; 9999 means the latest API
DEFINES   := -DQP_API_VERSION=9999

ifeq (,$(CONF))
	CONF := dbg
endif

#-----------------------------------------------------------------------------
# add QP-nano framework (depends on the OS this Makefile runs on):
#
ifeq ($(OS),Windows_NT)

QPN_PORT_DIR := $(QPN)/ports/win32-qv

C_SRCS += \
	qepn.c \
	qfn_win32.c \
	qwin_gui.c

else

QPN_PORT_DIR := $(QPN)/ports/posix-qv

C_SRCS += \
	qepn.c \
	qfn_posix.c

LIBS += -lpthread

endif

#============================================================================
# Typically you should not need to change anything below this line

VPATH    += $(QPN)/src/qfn $(QPN_PORT_DIR)
INCLUDES += -I$(QPN)/include -I$(QPN)/src -I$(QPN_PORT_DIR)

#-----------------------------------------------------------------------------
# GNU toolset:
#
# NOTE:
# GNU toolset (MinGW) is included in the QTools collection for Windows, see:
#     http://sourceforge.net/projects/qpc/files/QTools/
# It is assumed that %QTOOLS%\bin directory is added to the PATH
#
CC    := gcc
CPP   := g++
LINK  := gcc    # for C programs
#LINK  := g++   # for C++ programs

# NOTE:
# This Makefile assumes that the windres utility is available on the PATH
# (windres is available in the QTools collection for Windows)
#
RC    := windres

#-----------------------------------------------------------------------------
# basic utilities (depends on the OS this Makefile runs on):
#
ifeq ($(OS),Windows_NT)
	MKDIR      := mkdir
	RM         := rm
	TARGET_EXT := .exe
else ifeq ($(OSTYPE),cygwin)
	MKDIR      := mkdir -p
	RM         := rm -f
	TARGET_EXT := .exe
else
	MKDIR      := mkdir -p
	RM         := rm -f
	TARGET_EXT :=
endif

#-----------------------------------------------------------------------------
# build configurations...

ifeq (rel, $(CONF)) # Release configuration ..................................

BIN_DIR := build_rel

CFLAGS = -c -ffunction-sections -fdata-sections \
	-Os -Wall -W $(INCLUDES) $(DEFINES) -DNDEBUG

CPPFLAGS = -c -fno-rtti -fno-exceptions -ffunction-sections -fdata-sections \
	-Os -Wall -W $(INCLUDES) $(DEFINES) -DNDEBUG

else  # default Debug configuration ..........................................

BIN_DIR := build

CFLAGS = -c -g -ffunction-sections -fdata-sections \
	-O -Wall -W $(INCLUDES) $(DEFINES)

CPPFLAGS = -c -g -fno-rtti -fno-exceptions -ffunction-sections -fdata-sections \
	-O -Wall -W $(INCLUDES) $(DEFINES)

endif  # .....................................................................

LINKFLAGS :=

# is it a GUI application (any GUI resources provided?) ...
ifneq (,$(RC_SRCS))
LINKFLAGS += -mwindows
DEFINES += -DQWIN_GUI
endif

#-----------------------------------------------------------------------------
C_OBJS       := $(patsubst %.c,%.o,   $(C_SRCS))
CPP_OBJS     := $(patsubst %.cpp,%.o, $(CPP_SRCS))
RC_OBJS      := $(patsubst %.rc,  %.o, $(RC_SRCS))

TARGET_EXE   := $(BIN_DIR)/$(PROJECT)$(TARGET_EXT)
C_OBJS_EXT   := $(addprefix $(BIN_DIR)/, $(C_OBJS))
C_DEPS_EXT   := $(patsubst %.o,%.d, $(C_OBJS_EXT))
CPP_OBJS_EXT := $(addprefix $(BIN_DIR)/, $(CPP_OBJS))
CPP_DEPS_EXT := $(patsubst %.o,%.d, $(CPP_OBJS_EXT))
RC_OBJS_EXT  := $(addprefix $(BIN_DIR)/, $(RC_OBJS))

# create $(BIN_DIR) if it does not exist
ifeq ("$(wildcard $(BIN_DIR))","")
$(shell $(MKDIR) $(BIN_DIR))
endif

#-----------------------------------------------------------------------------
# rules
#

all: $(TARGET_EXE)

$(TARGET_EXE) : $(C_OBJS_EXT) $(CPP_OBJS_EXT) $(RC_OBJS_EXT)
	$(LINK) $(LINKFLAGS) $(LIB_DIRS) -o $@ $^ $(LIBS)

$(BIN_DIR)/%.d : %.cpp
	$(CPP) -MM -MT $(@:.d=.o) $(CPPFLAGS) $< > $@

$(BIN_DIR)/%.d : %.c
	$(CC) -MM -MT $(@:.d=.o) $(CFLAGS) $< > $@

$(BIN_DIR)/%.o : %.cpp
	$(CPP) $(CPPFLAGS) $< -o $@

$(BIN_DIR)/%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/%.o : %.rc
	$(RC) $(RCINCLUDES) -i $< -o $@

# include dependency files only if our goal depends on their existence
ifneq ($(MAKECMDGOALS),clean)
  ifneq ($(MAKECMDGOALS),show)
-include $(C_DEPS_EXT) $(CPP_DEPS_EXT)
  endif
endif

.PHONY : clean show

clean :
	-$(RM) $(BIN_DIR)/*.o \
	$(BIN_DIR)/*.d \
	$(TARGET_EXE)

show :
	@echo PROJECT      = $(PROJECT)
	@echo TARGET_EXE   = $(TARGET_EXE)
	@echo VPATH        = $(VPATH)
	@echo C_SRCS       = $(C_SRCS)
	@echo CPP_SRCS     = $(CPP_SRCS)
	@echo RC_SRCS      = $(RC_SRCS)
	@echo C_DEPS_EXT   = $(C_DEPS_EXT)
	@echo C_OBJS_EXT   = $(C_OBJS_EXT)
	@echo C_DEPS_EXT   = $(C_DEPS_EXT)
	@echo CPP_DEPS_EXT = $(CPP_DEPS_EXT)
	@echo CPP_OBJS_EXT = $(CPP_OBJS_EXT)
	@echo RC_OBJS_EXT  = $(RC_OBJS_EXT)
	@echo LIB_DIRS     = $(LIB_DIRS)
	@echo LIBS         = $(LIBS)
	@echo DEFINES      = $(DEFINES)

