#ifndef _COUT__H_
#define _COUT__H_

template<typename ...TS>
void Cout( TS const& ...parms );

template<typename ...TS>
void Coutn( TS const& ...parms );

template<typename ...TS>
void CoutPos( short x, short y, TS const& ...parms );

#endif
