LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/examples/TestSceneExample.cpp \
                   ../../Classes/httphelper_test/HttpHelperTest.cpp \
                   ../../Classes/dm_test/Serializable.cpp \
                   ../../Classes/dm_test/DMTest.cpp \
                   ../../Classes/clock_test/ClockTest.cpp \
                   ../../Classes/UnitTest/GlobalTestcases.cpp \
                   ../../Classes/UnitTest/UnitTestLayer.cpp \
                   ../../Classes/UnitTest/TestScene.cpp \
                   ../../Classes/UnitTest/VisibleRect.cpp \
                   ../../../module_clock/EverydayAward.cpp \
                   ../../../module_clock/IntervalAward.cpp \
                   ../../../module_clock/ntpdate.c \
                   ../../../module_clock/OnlineTimeAward.cpp \
                   ../../../module_clock/TimerManager.cpp \
                   ../../../module_data_manager/lookup_dict.cpp \
                   ../../../module_resources_manager/FontHelper.cpp \
                   ../../../module_resources_manager/ResourcesHelper.cpp \
                   ../../../module_utils/HttpHelper.cpp \
                   ../../../module_utils/StringUtil.cpp \
                   ../../../module_audio/AudioHelper.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../Classes/UnitTest \
					$(LOCAL_PATH)/../../Classes/dm_test \
					$(LOCAL_PATH)/../../Classes/clock_test \
					$(LOCAL_PATH)/../../Classes/httphelper_test \
					$(LOCAL_PATH)/../../Classes/examples \
					$(LOCAL_PATH)/../../../module_clock \
					$(LOCAL_PATH)/../../../module_data_manager \
					$(LOCAL_PATH)/../../../module_resources_manager \
					$(LOCAL_PATH)/../../../module_utils \
					$(LOCAL_PATH)/../../../module_audio \
					/Users/qiong/Documents/source/cocos2d-x-3.2/external


LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += aes_static

# LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static


include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, /Users/qiong/Documents/source/cocos2d-x-3.2/cocos)
$(call import-add-path, /Users/qiong/Documents/source/cocos2d-x-3.2)
$(call import-add-path, /Users/qiong/Documents/source/cocos2d-x-3.2/external)
$(call import-add-path, $(LOCAL_PATH)/../../../module_data_manager)

$(call import-module,.)
$(call import-module,audio/android)
$(call import-module,aes)

# $(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
# $(call import-module,editor-support/spine)
# $(call import-module,editor-support/cocostudio)
$(call import-module,network)
# $(call import-module,extensions)
