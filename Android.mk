LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

ifeq (,$(AVALON_FLAVOR))
$(error ERROR: variable AVALON_FLAVOR not set)
endif

LOCAL_MODULE := avalon_static
LOCAL_MODULE_FILENAME := libavalon

LOCAL_SRC_FILES := \
	avalon/i18n/Language.cpp \
	avalon/i18n/LanguageKey.cpp \
	avalon/i18n/Localization.cpp \
	avalon/io/IniReader.cpp \
	avalon/platform/android-$(AVALON_FLAVOR)/utils/platform.cpp

ifneq (,$(findstring utils,$(AVALON_FEATURES)))
LOCAL_SRC_FILES += \
	avalon/platform/android/utils/url.cpp
endif

ifneq (,$(findstring ads,$(AVALON_FEATURES)))
LOCAL_SRC_FILES += \
	avalon/ads/Manager.cpp \
	avalon/ads/provider/Chartboost.cpp \
	avalon/platform/android/ads/provider/ChartboostXJni.cpp \
	avalon/platform/android/ads/provider/ChartboostX_android.cpp \
	avalon/platform/android/ads/provider/Revmob.cpp
endif

ifneq (,$(findstring payment,$(AVALON_FEATURES)))
LOCAL_SRC_FILES += \
	avalon/payment/Loader.cpp \
	avalon/payment/Manager.cpp \
	avalon/payment/Product.cpp \
	avalon/payment/ProductConsumable.cpp \
	avalon/platform/android-$(AVALON_FLAVOR)/payment/Backend.cpp
endif

LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

# enable c++11 support but "remove" the override specifier with a simple
# preprocessor define - it's not supported yet :(
LOCAL_CFLAGS += -std=c++11 -Doverride=

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static

include $(BUILD_STATIC_LIBRARY)

$(call import-module,cocos2dx)
