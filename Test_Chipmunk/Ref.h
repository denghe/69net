#ifndef __REF_H__
#define __REF_H__

class Ref
{
public:
    void Retain();
    void Release();
    unsigned int refCount = 1;
    //function<void( Ref* p )>* deleter = nullptr;
protected:
    Ref();
    virtual ~Ref();
};

#endif
