IOS_CC_PATH=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin
IOS_SDK_PATH=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS7.0.sdk
IOS_ARCHI_TYPE = -arch armv7
IOS_BUILD_FLAG = $(IOS_ARCHI_TYPE) -isysroot $(IOS_SDK_PATH) -g -Wall -O3
CFLAG= -stdlib=libc++

#GXX = $(IOS_CC_PATH)/clang++
GXX = g++
CC = $(IOS_CC_PATH)/clang
AR = $(IOS_CC_PATH)/ar

all:aes libaes.a

aes:aes.c test/main.c
	$(GXX) -c $^
	$(GXX) -o $@ *.o
	rm *.o
libaes.a:aes_my.o aes.o
	$(AR) -crs libaes.a $^
	rm *.o
aes.o:aes.c
	$(CC) $(IOS_BUILD_FLAG) -c $(CFLAG) $<
aes_my.o:aes_my.c
	$(CC) $(IOS_BUILD_FLAG) -c $(CFLAG) $<
clean:
	rm *.o aes libaes.a