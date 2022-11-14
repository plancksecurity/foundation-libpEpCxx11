# Copyleft 2018 pEp foundation
#
# This file is under GNU General Public License 3.0
# see LICENSE.txt
 
LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)

LOCAL_MODULE    := pEpCxx11


LOCAL_CPP_FEATURES += exceptions
LOCAL_CPPFLAGS += -std=c++14 -DANDROID_STL=c++_shared -frtti

#FIXME: WORKARROUND 
STUB = $(shell sh $(LOCAL_PATH)/../takeOutHeaderFiles.sh $(LOCAL_PATH)/../../../pEpEngine/ $(LOCAL_PATH)/../../)
$(info $(STUB))

LIB_PEP_ADAPTER_INCLUDE_FILES := $(wildcard $(LOCAL_PATH)/../../src/*.h*)

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../pEpEngine/build-android/include \
            $(LIB_PEP_ADAPTER_INCLUDE_FILES:%=%)

LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)../include

LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/../../src/*.cc)


include $(BUILD_STATIC_LIBRARY)
