#ifndef RANDOM_H
#define RANDOM_H
namespace Lib3 {



// 随机函数类 (参考自poco, 只保留最大State那种)
//
struct Random {
    Random();                                                           // 初始化状态数据。以当前TickCount为种
    Random(UInt32 const &seed);                                         // 初始化时传入指定种子
	void                        SetSeed(UInt32 const &seed);            // 设置一个种子
	UInt32                      Next();                                 // 返回一个 0 ~ 0x7FFFFFFF 的 UInt32
	UInt32                      Next(UInt32 const &n);                  // 返回一个 0 ~ n && <= 0x7FFFFFFF 的 UInt32
	SByte                       NextChar();                             // 
    Char                        NextWChar();                            //
	Boolean                        NextBool();                             //
	Single                       NextFloat();                            //
	Double                      NextDouble();                           //
protected:
	void                        InitState(UInt32 const &seed);
	static UInt32               GoodRand(Int32 const &x);
private:
	UInt32                     *_fptr;
	UInt32                     *_rptr;
	UInt32                     *_state;
	UInt32                     *_endPtr;
	UInt32                      _pBuffer[64];
};




}
#endif