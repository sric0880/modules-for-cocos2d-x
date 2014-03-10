gyp aes.gyp --depth=. -f xcode -DOS=ios --generator-output=./
xcodebuild -project aes.xcodeproj
mv ./build/Default-iphoneos/libaes.a ./
rm -rf ./build
rm -rf aes.xcodeproj
gyp aes.gyp --depth=. -DOS=mac
xcodebuild -project aes.xcodeproj
mv ./build/Default/* ./
rm -rf ./build
rm -rf aes.xcodeproj