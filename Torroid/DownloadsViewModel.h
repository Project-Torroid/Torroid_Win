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
    private:
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
