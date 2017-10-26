#include "stdafx.h"
#include "Common.h"


namespace Reuse1
{

//---------------------------------------------------------------------------------
// GrandParent
//---------------------------------------------------------------------------------
class Parent 
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

    virtual void Step1(ObjectData* data)
    {
        for (int i = 0; i < ObjectData::kSize; ++i)
        {
            data->integers[i] = i;
        }
        Step2(data);
    }
    virtual void Step2(ObjectData* data)
    {
        data->sum = 0;
        for (int i : data->integers)
        {
            data->sum += i;
        }
    }

    virtual Parent::ObjectData* GetData() = 0;

    void Process()
    {
        ObjectData* data = GetData();
        Step1(data);
        data->Render();
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

    virtual void Step1(ObjectData* data) override
    {
        Parent::Step1(data);
        ChildData* pData = dynamic_cast<ChildData*>(data);
        for (int i = 0; i < ChildData::kSize; ++i)
        {
            pData->decimals[i] = i + i / 10.0f;
        }
    }

    virtual void Step2(ObjectData* data) override
    {
        Parent::Step2(data);
        ChildData* pData = dynamic_cast<ChildData*>(data);
        for (float i : pData->decimals)
        {
            pData->sum += (int)i;
        }
    }

    virtual Parent::ObjectData* GetData() override { return &m_data; }

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

    virtual void Step1(ObjectData* data) override
    {
        Parent::Step1(data);
        ChildData* pData = dynamic_cast<ChildData*>(data);
        for (int i = 0; i < ChildData::kSize; ++i)
        {
            pData->characters[i] = (char)i;
        }
    }

    virtual void Step2(ObjectData* data) override
    {
        Parent::Step2(data);
        ChildData* pData = dynamic_cast<ChildData*>(data);
        for (float i : pData->characters)
        {
            pData->sum += (int)i;
        }
    }

    virtual Parent::ObjectData* GetData() override { return &m_data; }

protected:
    ChildData m_data;
};



} // namespace reuse1



using namespace Reuse1;
void ReuseTests()
{
    printf("ReuseTests\n");
    std::shared_ptr<Parent> lParent = std::make_shared<LeftChild>();
    std::shared_ptr<Parent> rParent = std::make_shared<RightChild>();

    lParent->Process();
    rParent->Process();
    printf("\n");
}
