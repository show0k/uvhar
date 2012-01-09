SDK_PATH:=$(shell pwd)/../../ARDroneLib
PC_TARGET=yes

ifdef ALL_SOURCES
   include $(ARDRONE_CUSTOM_CONFIG)
   include $(ARDRONE_BUILD_CONFIG)
else
   include $(SDK_PATH)/Soft/Build/custom.makefile
   include $(SDK_PATH)/Soft/Build/config.makefile
endif

ifeq "$(RELEASE_BUILD)" "yes"
   ARDRONE_TARGET_DIR=$(shell pwd)/../Release
else
   ARDRONE_TARGET_DIR=$(shell pwd)/../Debug
endif

TARGET=uvhar

SRC_DIR:=$(shell pwd)/../Sources

# Define application source files
GENERIC_BINARIES_SOURCE_DIR:=$(SRC_DIR)

# source files of our program
GENERIC_BINARIES_COMMON_SOURCE_FILES+=			\
   UI/ui.c  \
   UI/gamepad.c \
   Navdata/navdata.c    \
   Video/video_stage.c

GENERIC_INCLUDES+=					\
	$(SRC_DIR) \
	$(LIB_DIR) \
	$(SDK_PATH)/Soft/Common \
	$(SDK_PATH)/Soft/Lib

GENERIC_TARGET_BINARIES_PREFIX=

GENERIC_TARGET_BINARIES_DIR=$(ARDRONE_TARGET_DIR)

GENERIC_BINARIES_SOURCE_ENTRYPOINTS+=			\
     uvhar.c

GENERIC_INCLUDES:=$(addprefix -I,$(GENERIC_INCLUDES))

GENERIC_LIB_PATHS=-L$(GENERIC_TARGET_BINARIES_DIR)
GENERIC_LIBS=-lpc_ardrone -lgtk-x11-2.0 -lrt -lpython2.6

SDK_FLAGS+="USE_APP=yes"
# this was "linux_sdk_demo"
SDK_FLAGS+="APP_ID=uvhar"

export GENERIC_CFLAGS
export GENERIC_LIBS
export GENERIC_LIB_PATHS
export GENERIC_INCLUDES
export GENERIC_BINARIES_SOURCE_DIR
export GENERIC_BINARIES_COMMON_SOURCE_FILES
export GENERIC_TARGET_BINARIES_PREFIX
export GENERIC_TARGET_BINARIES_DIR
export GENERIC_BINARIES_SOURCE_ENTRYPOINTS

# Bug fix ...
export GENERIC_LIBRARY_SOURCE_DIR=$(GENERIC_BINARIES_SOURCE_DIR)

.PHONY: $(TARGET) $(MAKECMDGOALS) build_libs

all: build_libs $(TARGET)

$(TARGET):
	@$(MAKE) -C $(SDK_PATH)/VP_SDK/Build $(TMP_SDK_FLAGS) $(SDK_FLAGS) $(MAKECMDGOALS)
	mv $(ARDRONE_TARGET_DIR)/uvhar $(TARGET)
	mv $(TARGET) $(ARDRONE_TARGET_DIR)/
	cp $(SRC_DIR)/uvhar.py $(ARDRONE_TARGET_DIR)
	

$(MAKECMDGOALS): build_libs
	@$(MAKE) -C $(SDK_PATH)/VP_SDK/Build $(TMP_SDK_FLAGS) $(SDK_FLAGS) $(MAKECMDGOALS)

build_libs:
	@$(MAKE) -C $(SDK_PATH)/Soft/Build $(TMP_SDK_FLAGS) $(SDK_FLAGS) $(MAKECMDGOALS)

