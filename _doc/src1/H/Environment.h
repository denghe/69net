#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
namespace Lib3 {
namespace Environment {


UInt64 GetTickCount();          // 取 硬件 tick count
UInt64 GetFrequency();          // 取 CPU 频率  (tick count 除以频率可以得到纳秒)
// UInt64 GetFrequency 这个需要查找 CPU 硬件信息或者 测试
// (取time1,取tick1,sleep(1),取tick2,取time2,算 time1,tick1 与 time2,tick2 的差值做除法)
// (测试法只适合少量调用, 毕竟会卡几十分之一秒。比如可以放在程序刚开始运行的时候取一次, 或第一次获取)


}



}
#endif
