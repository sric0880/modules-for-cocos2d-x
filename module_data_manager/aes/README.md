##使用方法
###1.	编译静态库
```
cd aes
make
```
使用makefile编译需要注意：根据需要配置“IOS_ARCHI_TYPE = -arch armv7”。makefile会生成一个ios的静态库，和一个可执行程序，效果和build_ios和build_executable一样。


推荐使用如下方法来生成静态库：

###iOS
```
./gen_ios_xcodeproj.sh
./build_ios
```
生成`build\Default-iphoneos\libaes.a`
###iOS simulator
```
./gen_ios_xcodeproj.sh
./build_iossim
```
生成`build\Default-iphonesimulator\libaes.a`
###Mac
```
./gen_mac_xcodeproj.sh
./build_mac
```
生成`build\Default\libaes.a`
###executable
```
./build_executable
```
在${Root}/tools/aes下生成两个文件：aes可执行文件，libaes.a静态库（mac）。

将编译好的静态库和相应的.h文件导入到自己的工程中。

###2.	导入.xcodeproj
或者执行
```
./gen_ios_xcodeproj.sh
或 ./gen_ios_xcodeproj.sh
```
直接将生成的Xcodeproj工程导入到自己的项目工程中。