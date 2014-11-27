IOS_CC_PATH=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin
IOS_SDK_PATH=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS7.0.sdk
IOS_ARCHI_TYPE = -arch armv7
IOS_BUILD_FLAG = $(IOS_ARCHI_TYPE) -isysroot $(IOS_SDK_PATH) -g -Wall -O3

CC = gcc
CLANG = $(IOS_CC_PATH)/clang
AR = $(IOS_CC_PATH)/ar
CFLAG = -D SQLITE_HAS_CODEC

all:sqlite3 sqlite3_test libsqlite3.a

sqlite3_test:main_test.c sqlite3secure.o
	$(CC) -c $< $(CFLAG)
	$(CC) -o $@ *.o
sqlite3:main.c sqlite3secure.c
	$(CC) -c $^ $(CFLAG)
	$(CC) -o $@ *.o
	rm main.o
libsqlite3.a:sqlite3secure.c
	rm *.o
	$(CLANG) $(IOS_BUILD_FLAG) -c $(CFLAG) $<
	$(AR) -crs libsqlite3.a *.o
clean:
	rm *.o sqlite3 sqlite3_test libsqlite3.a