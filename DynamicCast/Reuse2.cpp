#include "stdafx.h"
#include "Common.h"


namespace Reuse2
{


//---------------------------------------------------------------------------------
// IRobust
//---------------------------------------------------------------------------------
interface IDataProcessor
{
public:
    virtual void Process() = 0;
};


//---------------------------------------------------------------------------------
// GrandParent
//---------------------------------------------------------------------------------
class Parent : public IDataProcessor
{
public:
    class ObjectData 
    {
    public:
        static const int kSize = 15;
        int integers[kSize];
        int sum;

        virtual void Render()
        {
            for (int i : integers)
            {
                printf("%d, ", i);
            }
            printf("\n");
        }
    };

    void Step1(ObjectData* data)
    {
        for (int i = 0; i < ObjectData::kSize; ++i)
        {
            data->integers[i] = i;
        }
    }
    void Step2(ObjectData* data)
    {
        data->sum = 0;
        for (int i : data->integers)
        {
            data->sum += i;
        }
    }

};



//---------------------------------------------------------------------------------
// LeftParent
//---------------------------------------------------------------------------------
class LeftChild : public Parent
{
public:

    class ChildData : public Parent::ObjectData
    {
    public:
        static const int kSize = 15;
        float decimals[kSize];

        virtual void Render() override
        {
            Parent::ObjectData::Render();
            for (float i : decimals)
            {
                printf("%f, ", i);
            }
            printf("\n");
        }
    };

    void Step1()
    {
        Parent::Step1(&m_data);
        for (int i = 0; i < ChildData::kSize; ++i)
        {
            m_data.decimals[i] = i + i / 10.0f;
        }
    }

    void Step2()
    {
        Parent::Step2(&m_data);
        for (float i : m_data.decimals)
        {
            m_data.sum += (int)i;
        }
    }

    void Process() override
    {
        Step1();
        Step2();
        m_data.Render();
    }

protected:
    ChildData m_data;
};


//---------------------------------------------------------------------------------
// RightParent
//---------------------------------------------------------------------------------
class RightChild : public Parent
{
public:

    class ChildData : public Parent::ObjectData
    {
    public:
        static const int kSize = 15;
        char characters[kSize];

        virtual void Render() override
        {
            Parent::ObjectData::Render();
            for (char i : characters)
            {
                printf("%c, ", i+65);
            }
            printf("\n");
        }
    };

    void Step1()
    {
        Parent::Step1(&m_data);
        for (int i = 0; i < ChildData::kSize; ++i)
        {
            m_data.characters[i] = i + i / 10.0f;
        }
    }

    void Step2()
    {
        Parent::Step2(&m_data);
        for (float i : m_data.characters)
        {
            m_data.sum += (int)i;
        }
    }

    void Process() override
    {
        Step1();
        Step2();
        m_data.Render();
    }


protected:
    ChildData m_data;
};



} // namespace reuse2



using namespace Reuse2;
void ReuseTests2()
{
    printf("ReuseTests2\n");
    std::shared_ptr<Parent> lParent = std::make_shared<LeftChild>();
    std::shared_ptr<Parent> rParent = std::make_shared<RightChild>();


    lParent->Process();
    rParent->Process();
    printf("\n");
}