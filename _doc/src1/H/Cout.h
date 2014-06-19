#ifndef COUT_H
#define COUT_H
namespace Lib3 {



// 提供简化 Console 使用的一系列功能和运算符重载
const struct Cout_t {
	Cout_t() {
        Console::SetLocale();
    }
    template<typename T>            
    Cout_t const &                  operator<<(T const &in) const {
        Console::Write(in);
        return *this;
    }
    //template<typename T, UInt32 len>            
    //Cout_t const &                  operator<<(T const (&in)[len]) const {
    //    Console::Write(in, len - 1);
    //    return *this;
    //}
    template<typename T>            
    Cout_t const & Write(T const &in) const {
        Console::Write(in);
        return *this;
    }

    template<typename T>            
    Cout_t const & WriteLine(T const &in) const {
        Console::Write(in);
        Console::Write('\n');
        return *this;
    }

    Cout_t const & WriteLine() const {
        Console::Write('\n');
        return *this;
    }

} Cout;



}
#endif