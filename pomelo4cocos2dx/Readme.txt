对c版本的libpomelo进行了c++封装，支持cocos2dx主线程回调，类似HttpClient
只有一个类——PomeloClient，代码简洁易懂

1. 支持c++11特性，代码更加精简
2. 线程安全

依赖库：
1. libpomelo
    github: https://github.com/NetEase/libpomelo
    使用ios选项进行编译，生成.a文件只能用于真机调试
2. cocos2d-x-3.0beta2