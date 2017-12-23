## This file is part of the libopencm3 project.
##
## Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
##
## This library is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
## GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with this library. If not, see <http://www.gnu.org/licenses/>.
##

all: images

BUILD_HOME=../..

BINARY = main
LDLIBS += $(BUILD_HOME)/lib/pwm_lib.a
library: lib
	cp pwm_lib.a $(BUILD_HOME)/lib
lib: pwm_lib.o
	$(Q)$(AR) rcs -o pwm_lib.a -c pwm_lib.o

include $(BUILD_HOME)/src/Makefile.include
