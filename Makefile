##=====================================================================
## TimedPetriNetEditor: A timed Petri net editor.
## Copyright 2021 -- 2022 Quentin Quadrat <lecrapouille@gmail.com>
##
## This file is part of PetriEditor.
##
## PetriEditor is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
##=====================================================================

###################################################
# Project definition
#
PROJECT = GUI
TARGET = $(PROJECT)
DESCRIPTION = GUI
STANDARD = --std=c++14
BUILD_TYPE = debug

###################################################
# Location of the project directory and Makefiles
#
P := .
M := $(P)/.makefile
include $(M)/Makefile.header

###################################################
# Inform Makefile where to find header files
#
INCLUDES += -I$(P)/src -I$(P)

###################################################
# Inform Makefile where to find *.cpp and *.o files
#
VPATH += $(P)/src

###################################################
# Project defines
#
DEFINES += -DDATADIR=\"$(DATADIR)\"

###################################################
# Reduce warnings
#
DEFINES += -Wno-undef -Wno-switch-enum -Wno-sign-conversion -Wno-float-equal
DEFINES += -Wno-deprecated-copy-dtor -Wno-defaulted-function-deleted

###################################################
# Make the list of compiled files for the application
#
OBJS += Application.o MQTT.o main.o

###################################################
# Set SFML Library.
#
PKG_LIBS += sfml-graphics

###################################################
# Set MQTT Library.
#
DEFINES += -DMQTT_BROKER_ADDR=\"localhost\"
DEFINES += -DMQTT_BROKER_PORT=1883
PKG_LIBS += libmosquitto

LINKER_FLAGS += -lpthread

###################################################
# Compile the project, the static and shared libraries
.PHONY: all
all: $(TARGET)

###################################################
# Sharable informations between all Makefiles
include $(M)/Makefile.footer
