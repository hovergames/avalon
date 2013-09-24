LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# inject the used flavor into the c++ preprocessor. please keep this block
# up to date with <avalon/utils/platform.h> and <avalon/utils/platform.cpp>!
ifeq (amazon, $(AVALON_PLATFORM_FLAVOR))
    LOCAL_CFLAGS += -DAVALON_PLATFORM_FLAVOR=1
    LOCAL_EXPORT_CFLAGS += -DAVALON_PLATFORM_FLAVOR=1
else ifeq (google, $(AVALON_PLATFORM_FLAVOR))
    LOCAL_CFLAGS += -DAVALON_PLATFORM_FLAVOR=2
    LOCAL_EXPORT_CFLAGS += -DAVALON_PLATFORM_FLAVOR=2
else ifeq (samsung, $(AVALON_PLATFORM_FLAVOR))
    LOCAL_CFLAGS += -DAVALON_PLATFORM_FLAVOR=3
    LOCAL_EXPORT_CFLAGS += -DAVALON_PLATFORM_FLAVOR=3
else
    $(error ERROR: variable AVALON_PLATFORM_FLAVOR not set)
endif

LOCAL_MODULE := avalon_static
LOCAL_MODULE_FILENAME := libavalon

LOCAL_SRC_FILES := \
    avalon/i18n/Language.cpp \
    avalon/i18n/LanguageKey.cpp \
    avalon/i18n/Localization.cpp \
    avalon/io/IniReader.cpp \
    avalon/utils/platform.cpp \
    avalon/platform/android/ui/AlertNative.cpp \
    avalon/platform/android/utils/url.cpp \
    avalon/platform/android/appirater/Appirater.cpp \
    avalon/ui/Alert.cpp

ifneq (,$(findstring ads,$(AVALON_FEATURES)))
    LOCAL_SRC_FILES += \
        avalon/ads/Manager.cpp \
        avalon/platform/android/ads/provider/Chartboost.cpp \
        avalon/platform/android/ads/provider/Revmob.cpp \
        avalon/platform/android/ads/provider/SamsungAdHub.cpp

    ifeq (google, $(AVALON_PLATFORM_FLAVOR))
        LOCAL_SRC_FILES += \
            avalon/platform/android-google/ads/provider/TapForTap.cpp
    endif
endif

ifneq (,$(findstring payment,$(AVALON_FEATURES)))
    LOCAL_SRC_FILES += \
        avalon/payment/Loader.cpp \
        avalon/payment/Manager.cpp \
        avalon/payment/Product.cpp \
        avalon/payment/ProductConsumable.cpp \
        avalon/platform/android/payment/Backend.cpp
endif

ifneq (,$(findstring gamecenter,$(AVALON_FEATURES)))
    LOCAL_SRC_FILES += \
        avalon/platform/android-$(AVALON_PLATFORM_FLAVOR)/gamecenter/GameCenter.cpp
endif

LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

# enable c++11 support but "remove" the override specifier with a simple
# preprocessor define - it's not supported yet :(
LOCAL_CFLAGS += -std=c++11 -Doverride= -fexceptions

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static

include $(BUILD_STATIC_LIBRARY)

$(call import-module,cocos2dx)
