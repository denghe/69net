#ifndef GUID_HPP
#define GUID_HPP
namespace Lib3 {



template<typename CharType>
void Guid::SetByStr(CharType const * const &s) {
    assert ( s && s[8] == '-' && s[13] == '-' && s[18] == '-' && s[23] == '-' );
    _timeLow          = (Nibble(s[ 0]) << 28) |
                        (Nibble(s[ 1]) << 24) |
                        (Nibble(s[ 2]) << 20) |
                        (Nibble(s[ 3]) << 16) |
                        (Nibble(s[ 4]) << 12) |
                        (Nibble(s[ 5]) <<  8) |
                        (Nibble(s[ 6]) <<  4) |
                        (Nibble(s[ 7]) <<  0);
    _timeMid          = UInt16(
                        (Nibble(s[ 9]) << 12) |
                        (Nibble(s[10]) <<  8) |
                        (Nibble(s[11]) <<  4) |
                        (Nibble(s[12]) <<  0));
    _timeHiAndVersion = UInt16(
                        (Nibble(s[14]) << 12) |
                        (Nibble(s[15]) <<  8) |
                        (Nibble(s[16]) <<  4) |
                        (Nibble(s[17]) <<  0));
    _clockSeq         = UInt16(
                        (Nibble(s[19]) <<  4) |
                        (Nibble(s[20]) <<  0) |
                        (Nibble(s[21]) << 12) |
                        (Nibble(s[22]) <<  8));
    _node[0]   =  Byte((Nibble(s[24]) <<  4) |
                        (Nibble(s[25]) <<  0));
    _node[1]   =  Byte((Nibble(s[26]) <<  4) |
                        (Nibble(s[27]) <<  0));
    _node[2]   =  Byte((Nibble(s[28]) <<  4) |
                        (Nibble(s[29]) <<  0));
    _node[3]   =  Byte((Nibble(s[30]) <<  4) |
                        (Nibble(s[31]) <<  0));
    _node[4]   =  Byte((Nibble(s[32]) <<  4) |
                        (Nibble(s[33]) <<  0));
    _node[5]   =  Byte((Nibble(s[34]) <<  4) |
                        (Nibble(s[35]) <<  0));
}





INLINE Guid::Guid() {
}
INLINE Guid::Guid(SByte const * const &s) {
    SetByStr(s);
}
INLINE Guid::Guid(Char const * const &s) {
    SetByStr(s);
}
INLINE Guid::Guid(Byte const * const &buf) {
    Set(buf);
}
INLINE void Guid::Set(SByte const * const &s) {
    SetByStr(s);
}
INLINE void Guid::Set(Char const * const &s) {
    SetByStr(s);
}
INLINE void Guid::Set(Byte const * const &buf) {
    memcpy(this, buf, 16);
}
INLINE Guid &Guid::operator=(SByte const * const &s) {
    SetByStr(s);
    return *this;
}
INLINE Guid &Guid::operator=(Char const * const &s) {
    SetByStr(s);
    return *this;
}
INLINE Guid &Guid::operator=(Byte const * const &buf) {
    Set(buf);
    return *this;
}
INLINE void Guid::SetZero() {
    var p   = (Int32*)this;     // 因为起始地址不一定是 8 的倍数故不用 Int64 初始化
    p[0]    = 0;
    p[1]    = 0;
    p[2]    = 0;
    p[3]    = 0;
}
INLINE void Guid::SetVersion(Version const &version) {
	_timeHiAndVersion &= 0x0FFF;
	_timeHiAndVersion |= (version << 12);
	_clockSeq &= 0x3FFF;
	_clockSeq |= 0x8000;
}
INLINE Guid::Version Guid::GetVersion() const {
	return Version(_timeHiAndVersion >> 12);
}
INLINE Int32 Guid::GetVariant() const {
	Int32 v = _clockSeq >> 13;
	if ((v & 6) == 6)
		return v;
	else if (v & 4)
		return 2;
	else
		return 0;
}
INLINE Boolean Guid::IsNull() const {
	var p1  = (Int32*)this;
	return  p1[0] == 0 &&
            p1[1] == 0 &&
            p1[2] == 0 &&
            p1[3] == 0;
}
INLINE Boolean Guid::operator==(Guid const &guid) const {
    var p1  = (Int32*)this;
    var p2  = (Int32*)&guid;
	return  p1[0] == p2[0] &&
            p1[1] == p2[1] &&
            p1[2] == p2[2] &&
            p1[3] == p2[3];
}
INLINE Boolean Guid::operator!=(Guid const &guid) const {
	return !operator==(guid);
}
INLINE Boolean Guid::operator<(Guid const &guid) const {
	return Compare(guid) < 0;
}
INLINE Boolean Guid::operator<=(Guid const &guid) const {
	return Compare(guid) <= 0;
}
INLINE Boolean Guid::operator>(Guid const &guid) const {
	return Compare(guid) > 0;
}
INLINE Boolean Guid::operator>=(Guid const &guid) const {
	return Compare(guid) >= 0;
}
INLINE Guid::Guid(UInt32 const &timeLow, UInt16 const &timeMid, UInt16 const &timeHiAndVersion, UInt16 const &clockSeq, Byte const * const &node) {
	_timeLow            = timeLow;
	_timeMid            = timeMid;
	_timeHiAndVersion   = timeHiAndVersion;
	_clockSeq           = clockSeq;
    _node[0]            = node[0];
    _node[1]            = node[1];
    _node[2]            = node[2];
    _node[3]            = node[3];
    _node[4]            = node[4];
    _node[5]            = node[5];
}
INLINE Int32 Guid::Compare(Guid const &guid) const {
	if (_timeLow != guid._timeLow)      return _timeLow < guid._timeLow ? -1 : 1;
	if (_timeMid != guid._timeMid)      return _timeMid < guid._timeMid ? -1 : 1;
	if (_timeHiAndVersion != guid._timeHiAndVersion) return _timeHiAndVersion < guid._timeHiAndVersion ? -1 : 1;
	if (_clockSeq != guid._clockSeq)    return _clockSeq < guid._clockSeq ? -1 : 1;
    if (_node[0] != guid._node[0])      return _node[0] < guid._node[0] ? -1 : 1;
    if (_node[1] != guid._node[1])      return _node[1] < guid._node[1] ? -1 : 1;
    if (_node[2] != guid._node[2])      return _node[2] < guid._node[2] ? -1 : 1;
    if (_node[3] != guid._node[3])      return _node[3] < guid._node[3] ? -1 : 1;
    if (_node[4] != guid._node[4])      return _node[4] < guid._node[4] ? -1 : 1;
    if (_node[5] != guid._node[5])      return _node[5] < guid._node[5] ? -1 : 1;
	return 0;
}
INLINE UInt32 Guid::Nibble(UInt32 const &c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	else if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	else if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	else
		return 0;
}





// 用于创建 Guid (里面存有环境信息，如 static random, 网卡mac地址之类, 生成时会 lock)
// 另一个考虑是把 Create 做到 Guid::NewGuid()  函数，内部用 static 声明变量存上述数据
// 


INLINE Guid Guid::NewGuid() {
#ifdef __linux
    // 取网卡 MAC 地址。如果没有网卡，就杜撰一个，同时也认为网卡 MAC 地址已取到
    //static Mutex mutex;           // todo
    //mutex.Lock();                 // todo // 这里就用 mutex. 锁定时间不算短了（计算 tv 全过程）
    static Byte mac[6];
    static Boolean inited = false;  // todo: GCC 有 attribute 可标记函数内静态的线程安全
    static Random rnd;
    if (!inited) {
        // todo: 取网卡 MAC 地址  先用随机数仿个
        Int32 ii[2];
        ii[0] = rnd.Next();
        ii[1] = rnd.Next();
        var p = (Byte*)ii;
        mac[0] = p[0];
        mac[1] = p[1];
        mac[2] = p[2];
        mac[3] = p[3];
        mac[4] = p[4];
        mac[5] = p[5];
        inited = true;
    }
    // 保存上一次时间值，因为取时间是有精度的，短期内反复取时间会重复
    // 为避免重复，如果发现取到的时间与上次相同，就递增一个修正 ticks，避免重复
	static DateTime _lastTime;
    static UInt32 _ticks;
	DateTime now;
    now.Update();
	for (;;) {
		if (now != _lastTime) {
			_lastTime = now;
			_ticks = 0;
			break;
		}
		if (_ticks < 100) {
			++_ticks;
			break;
		}
		now.Update();
	}
    //mutex.UnLock();               // todo
	var tv = now.Value + _ticks;             // 最终得到时间因子，开始填值
	UInt32 timeLow = UInt32(tv & 0xFFFFFFFF);
	UInt16 timeMid = UInt16((tv >> 32) & 0xFFFF);
	UInt16 timeHiAndVersion = UInt16((tv >> 48) & 0x0FFF) + (UUID_TIME_BASED << 12);
	UInt16 clockSeq = (UInt16(rnd.Next() >> 4) & 0x3FFF) | 0x8000;
	return Guid(timeLow, timeMid, timeHiAndVersion, clockSeq, mac);
#else
    Guid g;                         // windows 直接调系统函数来生成
    ::CoCreateGuid((GUID*)&g);      // if (S_OK == 
    return g;
#endif
}





}
#endif