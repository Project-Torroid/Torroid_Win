#include "pch.h"
#include "DownloadsViewModel.h"
#include "DownloadsViewModel.g.cpp"

using namespace winrt;
using namespace Windows::Foundation::Collections;

namespace winrt::Torroid::implementation
{
    DownloadsViewModel::DownloadsViewModel()
    {
        m_downloadsOBVector = single_threaded_observable_vector<Torroid::Downloads>();
    }

    Torroid::Downloads DownloadsViewModel::Downloads() {
        return m_downloads;
    }

    IObservableVector<Torroid::Downloads> DownloadsViewModel::DownloadsOBVector()
    {
        return m_downloadsOBVector;
    }
}
