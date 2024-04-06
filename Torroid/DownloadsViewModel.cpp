#include "pch.h"
#include "DownloadsViewModel.h"
#include "DownloadsViewModel.g.cpp"

namespace winrt::Torroid::implementation
{
    DownloadsViewModel::DownloadsViewModel()
    {
        m_downloads = winrt::make<Torroid::implementation::Downloads>(L"Hello World.mp4");
        m_downloadsOBVector = winrt::single_threaded_observable_vector<Torroid::Downloads>();
        m_downloadsOBVector.Append(m_downloads);
    }

    Torroid::Downloads DownloadsViewModel::Downloads() {
        return m_downloads;
    }
    Windows::Foundation::Collections::IObservableVector<winrt::Torroid::Downloads> DownloadsViewModel::DownloadsOBVector()
    {
        return m_downloadsOBVector;
    }
}
