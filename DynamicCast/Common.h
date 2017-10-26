#pragma once

#define interface class

//---------------------------------------------------------------------------------
// IRobust
//---------------------------------------------------------------------------------
interface IRobust
{
    // Can't break this interface.
};

//---------------------------------------------------------------------------------
// INamed
//---------------------------------------------------------------------------------
interface INamed
{
    virtual void PrintName() const = 0;
};



//---------------------------------------------------------------------------------
// ReflectionObject
//---------------------------------------------------------------------------------
class ReflectionObject
{
public:
    ReflectionObject() : m_type(s_typeCounter++) { }
    inline int GetType() const  { return m_type; }

    int m_type;
protected:
    static int s_typeCounter;
};


#define REFLECTION_OBJECT()                                             \
public:                                                                 \
    __forceinline static int GetStaticType() { return m_type.m_type; }  \
    __forceinline virtual int GetType()      { return GetStaticType(); }\
protected:                                                              \
    static ReflectionObject m_type; 






void ReuseTests();
void ReuseTests2();
void ReuseTests3();
