gyp safe_sqlite3.gyp --depth=. -f xcode -DOS=ios --generator-output=./
xcodebuild -project safe_sqlite3.xcodeproj
mv ./build/Default-iphoneos/libsqlite3.a ./
rm -rf ./build
rm -rf safe_sqlite3.xcodeproj
gyp safe_sqlite3.gyp --depth=. -DOS=mac
xcodebuild -project safe_sqlite3.xcodeproj
mv ./build/Default/* ./
rm -rf ./build
rm -rf safe_sqlite3.xcodeproj