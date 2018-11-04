 ///
 /// @file    Noncopyable.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-07-31 13:05:34
 ///
#ifndef __ZCL_NONCOPYABLE_H__
#define __ZCL_NONCOPYABLE_H__

namespace zcl
{

class Noncopyable
{
protected:
	Noncopyable(){}
	~Noncopyable(){}

private:
	Noncopyable(const Noncopyable &);
	Noncopyable & operator=(const Noncopyable)&;
};


}//end of namespace zcl

#endif
