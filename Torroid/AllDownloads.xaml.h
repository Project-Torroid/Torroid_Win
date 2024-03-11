#pragma once

#include "AllDownloads.g.h"

namespace winrt::Torroid::implementation
{
    struct AllDownloads : AllDownloadsT<AllDownloads>
    {
        AllDownloads()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);
        void AllDownloads::addDownloadBtnClicked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);

    };
}

namespace winrt::Torroid::factory_implementation
{
    struct AllDownloads : AllDownloadsT<AllDownloads, implementation::AllDownloads>
    {
    };
}
