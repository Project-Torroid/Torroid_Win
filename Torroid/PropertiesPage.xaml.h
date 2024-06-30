#pragma once

#include "PropertiesPage.g.h"

namespace winrt::Torroid::implementation
{
    struct PropertiesPage : PropertiesPageT<PropertiesPage>
    {
        PropertiesPage(hstring fName, hstring fSize);

        int32_t MyProperty();
        void MyProperty(int32_t value);
    };
}

namespace winrt::Torroid::factory_implementation
{
    struct PropertiesPage : PropertiesPageT<PropertiesPage, implementation::PropertiesPage>
    {
    };
}
