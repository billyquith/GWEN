/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/CollapsibleList.h>

namespace Gwk {
namespace Test {

class CollapsibleList:public ControlClass<CollapsibleList, TestUnit>
{
public:

    CollapsibleList(Gwk::Controls::Base *parent, const Gwk::String &name=""):BaseClass(parent, name)
    {
        Gwk::Controls::CollapsibleList* control = new Gwk::Controls::CollapsibleList(this);
        control->SetSize(100, 200);
        control->SetPos(10, 10);
        {
            Gwk::Controls::CollapsibleCategory* cat = control->Add("Category One");
            cat->Add("Hello");
            cat->Add("Two");
            cat->Add("Three");
            cat->Add("Four");
        }
        {
            Gwk::Controls::CollapsibleCategory* cat = control->Add("Shopping");
            cat->Add("Special");
            cat->Add("Two Noses");
            cat->Add("Orange ears");
            cat->Add("Beer");
            cat->Add("Three Eyes");
            cat->Add("Special");
            cat->Add("Two Noses");
            cat->Add("Orange ears");
            cat->Add("Beer");
            cat->Add("Three Eyes");
            cat->Add("Special");
            cat->Add("Two Noses");
            cat->Add("Orange ears");
            cat->Add("Beer");
            cat->Add("Three Eyes");
        }
        {
            Gwk::Controls::CollapsibleCategory* cat = control->Add("Category One");
            cat->Add("Hello");
            cat->Add("Two");
            cat->Add("Three");
            cat->Add("Four");
        }
    }
};

GWK_REGISTER_TEST(CollapsibleList);

}}

