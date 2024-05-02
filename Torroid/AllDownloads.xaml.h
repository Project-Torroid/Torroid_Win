#pragma once

#include "AllDownloads.g.h"
#include "DownloadsViewModel.h"

namespace winrt::Torroid::implementation
{
    struct AllDownloads : AllDownloadsT<AllDownloads>
    {
        AllDownloads();

        Torroid::DownloadsViewModel MainViewModel();

        void AllDownloads::addDownloadBtnClicked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);

        void AllDownloads::addToDownloadsLV(int index, bool isFirstCall);
        
        void AllDownloads::PlayPauseButton_Clicked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    private:
        Torroid::DownloadsViewModel m_mainViewModel;
    };
}

namespace winrt::Torroid::factory_implementation
{
    struct AllDownloads : AllDownloadsT<AllDownloads, implementation::AllDownloads>
    {
    };
}
