#ifndef RANDOM_HPP
#define RANDOM_HPP
namespace Lib3 {


INLINE Random::Random() {
	InitState((UInt32)Environment::GetTickCount());
}
INLINE Random::Random(UInt32 const &seed) {
	InitState(seed);
}
INLINE void Random::InitState(UInt32 const &seed) {
	_state  = _pBuffer + 1;
	_endPtr = &_state[63];
	SetSeed(seed);
	_pBuffer[0] = 5 * (Int32) (_rptr - _state) + 4;
}
INLINE UInt32 Random::Next(UInt32 const &n) {
	return Next() % n;
}
INLINE SByte Random::NextChar() {
	return SByte((Next() >> 3) & 0xFF);
}
INLINE Char Random::NextWChar() {
    if (sizeof(Char) == 2)
	    return Char((Next() >> 2) & 0xFFFF);
    else
        return Char(Next());
}
INLINE Boolean Random::NextBool() {
	return (Next() & 0x1000) != 0;
}
INLINE Single Random::NextFloat() {
	return Single(Next()) / 0x7FFFFFFF;
}
INLINE Double Random::NextDouble() {
	return Double(Next()) / 0x7FFFFFFF;
}



INLINE UInt32 Random::GoodRand(Int32 const &x) {
    var x_ = x;
	Int32 hi, lo;
	if (x_ == 0) x_ = 123459876;
	hi = x_ / 127773;
	lo = x_ % 127773;
	x_ = 16807 * lo - 2836 * hi;
	if (x_ < 0) x_ += 0x7FFFFFFF;
	return x_;
}
INLINE void Random::SetSeed(UInt32 const &x) {
	Int32 i, lim;
	_state[0] = x;
	for (i = 1; i < 63; i++)
		_state[i] = GoodRand(_state[i - 1]);
	_fptr = &_state[1];
	_rptr = &_state[0];
	lim = 10 * 63;
	for (i = 0; i < lim; i++)
		Next();
}
INLINE UInt32 Random::Next() {
	UInt32 i;
	UInt32 *f, *r;
	f = _fptr; r = _rptr;
	*f += *r;
	i = (*f >> 1) & 0x7FFFFFFF;
	if (++f >= _endPtr) {
		f = _state;
		++r;
	}
	else if (++r >= _endPtr)
		r = _state;
	_fptr = f; _rptr = r;
	return i;
}


}
#endif