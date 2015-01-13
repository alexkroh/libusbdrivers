#
# Copyright 2014, NICTA
#
# This software may be distributed and modified according to the terms of
# the BSD 2-Clause license. Note that NO WARRANTY is provided.
# See "LICENSE_BSD2.txt" for details.
#
# @TAG(NICTA_BSD)
#

# Targets
TARGETS := libusbdrivers.a

# Source files required to build the target
CFILES += $(patsubst $(SOURCE_DIR)/%,%,$(wildcard ${SOURCE_DIR}/src/plat/$(PLAT)/*.c))
CFILES += $(patsubst $(SOURCE_DIR)/%,%,$(wildcard ${SOURCE_DIR}/src/*.c))
CFILES += $(patsubst $(SOURCE_DIR)/%,%,$(wildcard ${SOURCE_DIR}/src/ehci/*.c))
CFILES += $(patsubst $(SOURCE_DIR)/%,%,$(wildcard ${SOURCE_DIR}/src/drivers/*.c))

# Header files/directories this library provides
HDRFILES := \
    $(wildcard ${SOURCE_DIR}/include/*) \
    $(wildcard ${SOURCE_DIR}/plat_include/$(PLAT)/*) \
    $(wildcard ${SOURCE_DIR}/arch_include/${ARCH}/*)

INCLUDE_DIRS+=$(SOURCE_DIR)/src/include
#INCLUDE_DIRS+=$(SOURCE_DIR)/src/drivers

include $(SEL4_COMMON)/common.mk