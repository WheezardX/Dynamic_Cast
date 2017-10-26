// DynamicCast.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Common.h"

 
int ReflectionObject::s_typeCounter = 0;


//---------------------------------------------------------------------------------
// GrandParent
//---------------------------------------------------------------------------------
class GrandParent : public IRobust, public INamed
{
    REFLECTION_OBJECT()
public:

    virtual void PrintName() const override { printf("GrandParent "); }

protected:
};
ReflectionObject GrandParent::m_type;


//---------------------------------------------------------------------------------
// LeftParent
//---------------------------------------------------------------------------------
class LeftParent : public GrandParent
{
    REFLECTION_OBJECT()
public:
    static LeftParent* DownCast(GrandParent* gp) 
    {
        bool typeMatch = gp->GetType() == LeftParent::GetStaticType();
        return typeMatch ? static_cast<LeftParent*>(gp) : nullptr;
    }

    virtual void PrintName() const  override { printf("LeftParent "); }

protected:

};
ReflectionObject LeftParent::m_type;


//---------------------------------------------------------------------------------
// LeftLeftChild
//---------------------------------------------------------------------------------
class LeftLeftChild : public LeftParent
{
    REFLECTION_OBJECT()
public:
    static LeftLeftChild* DownCast(GrandParent* gp) 
    {
        bool typeMatch = gp->GetType() == LeftLeftChild::GetStaticType();
        return typeMatch ? static_cast<LeftLeftChild*>(gp) : nullptr;
    }

    virtual void PrintName() const  override { printf("LeftLeftChild "); }

};
ReflectionObject LeftLeftChild::m_type;

//---------------------------------------------------------------------------------
// LeftRightChild
//---------------------------------------------------------------------------------
class LeftRightChild : public LeftParent
{
    REFLECTION_OBJECT()
public:
    static LeftRightChild* DownCast(GrandParent* gp) 
    {
        bool typeMatch = gp->GetType() == LeftRightChild::GetStaticType();
        return typeMatch ? static_cast<LeftRightChild*>(gp) : nullptr;
    }

    virtual void PrintName() const  override { printf("LeftRightChild "); }

};
ReflectionObject LeftRightChild::m_type;



//---------------------------------------------------------------------------------
// RightParent
//---------------------------------------------------------------------------------
class RightParent : public GrandParent
{
    REFLECTION_OBJECT()
public:
    static RightParent* DownCast(GrandParent* gp) 
    {
        bool typeMatch = gp->GetType() == RightParent::GetStaticType();
        return typeMatch ? static_cast<RightParent*>(gp) : nullptr;
    }

    virtual void PrintName() const  override { printf("RightParent "); }

protected:
};
ReflectionObject RightParent::m_type;

//---------------------------------------------------------------------------------
// RightLeftChild
//---------------------------------------------------------------------------------
class RightLeftChild : public RightParent
{
    REFLECTION_OBJECT()
public:
    static RightLeftChild* DownCast(GrandParent* gp) 
    {
        bool typeMatch = gp->GetType() == RightLeftChild::GetStaticType();
        return typeMatch ? static_cast<RightLeftChild*>(gp) : nullptr;
    }

    virtual void PrintName() const  override { printf("RightLeftChild "); }
};
ReflectionObject RightLeftChild::m_type;

//---------------------------------------------------------------------------------
// RightRightChild
//---------------------------------------------------------------------------------
class RightRightChild : public RightParent
{
    REFLECTION_OBJECT()
public:
    static RightRightChild* DownCast(GrandParent* gp) 
    {
        bool typeMatch = gp->GetType() == RightRightChild::GetStaticType();
        return typeMatch ? static_cast<RightRightChild*>(gp) : nullptr;
    }

    virtual void PrintName() const  override { printf("RightRightChild "); }
};
ReflectionObject RightRightChild::m_type;

//---------------------------------------------------------------------------------
// GrandChild
//---------------------------------------------------------------------------------
class GrandChild : public RightRightChild
{
    REFLECTION_OBJECT()
public:
    static GrandChild* DownCast(GrandParent* gp) 
    {
        bool typeMatch = gp->GetType() == GrandChild::GetStaticType();
        return typeMatch ? static_cast<GrandChild*>(gp) : nullptr;
    }

    virtual void PrintName() const  override { printf("GrandChild "); }
    void PrintNameNonVirtual() const         { printf("GrandChild "); }
};
ReflectionObject GrandChild::m_type;







using namespace std::chrono;
//---------------------------------------------------------------------------------
// CastCompares
//---------------------------------------------------------------------------------
static void CastCompare(GrandParent* grandParent, bool printName)
{
    const int kLoopCount = 1000;
    printf("----------------\nCastCompare %d\n-------------", kLoopCount);
    std::this_thread::sleep_for (std::chrono::milliseconds(1));
    double dynamic, down;
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        for (int i = 0; i < kLoopCount; ++i)
        {
            GrandChild* grandChild = dynamic_cast<GrandChild*>(grandParent);
            if (grandChild != nullptr && printName)
            {
                grandChild->PrintName();
            }
        }
  
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
        printf("-----\n Dynamic_Cast took %0.5f ms \n", fp_ms.count());
        dynamic = fp_ms.count();
    }

    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        for (int i = 0; i < kLoopCount; ++i)
        {
            GrandChild* grandChild = GrandChild::DownCast(grandParent);
            if (grandChild != nullptr && printName)
            {
                grandChild->PrintName();
            }
        }
  
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
        printf("-----\n DownCast took %0.5f ms \n", fp_ms.count());
        down = fp_ms.count();
    }
    printf("Delta %0.5f    Ratio %0.5f\n\n", dynamic - down, dynamic / down);
}

static void CastCompareWithFailure(GrandParent* grandParent, bool printName)
{
    const int kLoopCount = 1000;
    printf("----------------\nCastCompareWithFailure %d\n-------------", kLoopCount);
    std::this_thread::sleep_for (std::chrono::milliseconds(1));
    double dynamic, down;
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        for (int i = 0; i < kLoopCount; ++i)
        {
            GrandChild* grandChild = dynamic_cast<GrandChild*>(grandParent);
            if (grandChild != nullptr && printName)
            {
                grandChild->PrintName();
            }
            LeftParent* leftParent  = dynamic_cast<LeftParent*>(grandParent);
            if (leftParent != nullptr && printName)
            {
                leftParent->PrintName();
            }
        }
  
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
        printf("-----\n Dynamic_Cast took %0.5f ms \n", fp_ms.count());
        dynamic = fp_ms.count();
    }

    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        for (int i = 0; i < kLoopCount; ++i)
        {
            GrandChild* grandChild = GrandChild::DownCast(grandParent);
            if (grandChild != nullptr && printName)
            {
                grandChild->PrintName();
            }
            LeftParent* leftParent  = LeftParent::DownCast(grandParent);
            if (leftParent != nullptr && printName)
            {
                leftParent->PrintName();
            }
        }
  
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
        printf("-----\n DownCast took %0.5f ms \n", fp_ms.count());
        down = fp_ms.count();
    }
    printf("Delta %0.5f    Ratio %0.5f\n\n", dynamic - down, dynamic / down);
}

static void RandomCast(GrandParent* grandParent)
{
    const int kLoopCount = 100;
    printf("----------------\nRandomCast %d\n-------------\n", kLoopCount);
    std::this_thread::sleep_for (std::chrono::milliseconds(1));
    double dynamic, down;
    {
        srand(1234);
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        for (int i = 0; i < kLoopCount; ++i)
        {
            int val = rand() % 4;
            printf("%d", val);
            switch (val)
            {
                case 0:
                {
                    GrandChild* grandChild = dynamic_cast<GrandChild*>(grandParent);
                    if (grandChild != nullptr)
                    {
                        grandChild->PrintName();
                    }
                    else
                    {
                        printf("dynamic failure. ");
                    }
                }
                break;

                case 1:
                {
                    LeftParent* parent = dynamic_cast<LeftParent*>(grandParent);
                    if (parent != nullptr)
                    {
                        parent->PrintName();
                    }
                    else
                    {
                        printf("dynamic failure. ");
                    }
                }
                break;

                case 2:
                {
                    LeftLeftChild* parent = dynamic_cast<LeftLeftChild*>(grandParent);
                    if (parent != nullptr)
                    {
                        parent->PrintName();
                    }
                    else
                    {
                        printf("dynamic failure. ");
                    }
                }
                break;

                case 3:
                {
                    RightLeftChild* child = dynamic_cast<RightLeftChild*>(grandParent);
                    if (child != nullptr)
                    {
                        child->PrintName();
                    }
                    else
                    {
                        printf("dynamic failure. ");
                    }
                }
                break;
            };
        }
  
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
        printf("-----\n Dynamic_Cast took %0.5f ms \n\n", fp_ms.count());
        dynamic = fp_ms.count();
    }

    {
        srand(1234);
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        for (int i = 0; i < kLoopCount; ++i)
        {
            int val = rand() % 4;
            printf("%d", val);
            switch (val)
            {
                case 0:
                {
                    GrandChild* grandChild = GrandChild::DownCast(grandParent);
                    if (grandChild != nullptr)
                    {
                        grandChild->PrintName();
                    }
                    else
                    {
                        printf("dynamic failure. ");
                    }
                }
                break;

                case 1:
                {
                    LeftParent* parent = LeftParent::DownCast(grandParent);
                    if (parent != nullptr)
                    {
                        parent->PrintName();
                    }
                    else
                    {
                        printf("dynamic failure. ");
                    }
                }
                break;

                case 2:
                {
                    LeftLeftChild* parent = LeftLeftChild::DownCast(grandParent);
                    if (parent != nullptr)
                    {
                        parent->PrintName();
                    }
                    else
                    {
                        printf("dynamic failure. ");
                    }
                }
                break;

                case 3:
                {
                    RightLeftChild* child = RightLeftChild::DownCast(grandParent);
                    if (child != nullptr)
                    {
                        child->PrintName();
                    }
                    else
                    {
                        printf("dynamic failure. ");
                    }
                }
                break;
            };
        }
  
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
        printf("-----\n DownCast took %0.5f ms \n\n", fp_ms.count());
        down = fp_ms.count();
    }
    printf("Delta %0.5f    Ratio %0.5f\n\n", dynamic - down, dynamic / down);
}


//---------------------------------------------------------------------------------
// Interface compare
//---------------------------------------------------------------------------------
static void InterfaceCompare(GrandParent* grandParent, bool printName)
{
    const int kLoopCount = 100;
    printf("----------------\nInterfaceCompare  %d\n-------------\n", kLoopCount);
    std::this_thread::sleep_for (std::chrono::milliseconds(1));
    double dynamic, down;
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        for (int i = 0; i < kLoopCount; ++i)
        {
            GrandChild* grandChild = dynamic_cast<GrandChild*>(grandParent);
            if (grandChild != nullptr && printName)
            {
                grandChild->PrintNameNonVirtual();
            }
        }
  
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
        printf("-----\n Dynamic_Cast took %0.5f ms \n", fp_ms.count());
        dynamic = fp_ms.count();
    }

    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        for (int i = 0; i < kLoopCount; ++i)
        {
            if (printName)
            {
                grandParent->PrintName();
            }
        }
  
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
        printf("-----\n DownCast took %0.5f ms \n", fp_ms.count());
        down = fp_ms.count();
    }
    printf("Delta %0.5f    Ratio %0.5f\n\n", dynamic - down, dynamic / down);
}


static void InterfaceCompareVirtual(GrandParent* grandParent, bool printName)
{
    const int kLoopCount = 100;
    printf("----------------\nInterfaceCompareVirtual %d \n-------------\n", kLoopCount);
    std::this_thread::sleep_for (std::chrono::milliseconds(1));
    double dynamic, down;
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        for (int i = 0; i < kLoopCount; ++i)
        {
            GrandChild* grandChild = dynamic_cast<GrandChild*>(grandParent);
            if (grandChild != nullptr && printName)
            {
                grandChild->PrintName();
            }
        }
  
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
        printf("-----\n Dynamic_Cast took %0.5f ms \n", fp_ms.count());
        dynamic = fp_ms.count();
    }

    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        for (int i = 0; i < kLoopCount; ++i)
        {
            if (printName)
            {
                grandParent->PrintName();
            }
        }
  
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
        printf("-----\n DownCast took %0.5f ms \n", fp_ms.count());
        down = fp_ms.count();
    }
    printf("Delta %0.5f    Ratio %0.5f\n\n", dynamic - down, dynamic / down);
}












int main()
{
    ReuseTests();
    ReuseTests2();
    ReuseTests3();

    auto grandChild = std::make_shared<GrandChild>();
    LeftParent* parent1 = dynamic_cast<LeftParent*>(grandChild.get());
    LeftParent* parent2 = LeftParent::DownCast(grandChild.get());
    if (parent1 != nullptr || parent2 != nullptr)
    {
        assert(false);
    }

    for (int i = 0; i < 5; ++i)
    {
        CastCompare(grandChild.get(), false);
        CastCompareWithFailure(grandChild.get(), false);
        RandomCast(grandChild.get());

        InterfaceCompare(grandChild.get(), false);
        InterfaceCompareVirtual(grandChild.get(), false);

        int x;
        std::cin >> x;
    }


    int x;
    std::cin >> x;

    return 0;
}

