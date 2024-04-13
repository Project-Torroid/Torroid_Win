#pragma once
#include "DownloadsViewModel.g.h"
#include "Downloads.h"

namespace winrt::Torroid::implementation
{
    struct DownloadsViewModel : DownloadsViewModelT<DownloadsViewModel>
    {
        DownloadsViewModel();

        winrt::Torroid::Downloads Downloads();

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::Torroid::Downloads> DownloadsOBVector();

        void DownloadsViewModel::startDownloadStatsUpdates();
        void DownloadsViewModel::stopDownloadStatsUpdates();

        void DownloadsViewModel::OnTick(Windows::Foundation::IInspectable const& /* sender */,
            Windows::Foundation::IInspectable const& /* e */);

        void DownloadsViewModel::updateDownloadStats(int index);

    private:
        Microsoft::UI::Xaml::DispatcherTimer timer;
        Torroid::Downloads m_downloads{ nullptr };
        Windows::Foundation::Collections::IObservableVector<Torroid::Downloads> m_downloadsOBVector;
    };
}
namespace winrt::Torroid::factory_implementation
{
    struct DownloadsViewModel : DownloadsViewModelT<DownloadsViewModel, implementation::DownloadsViewModel>
    {
    };
}
