#include "pch.h"
#include "DownloadsViewModel.h"
#include "DownloadsViewModel.g.cpp"

#include "DownloadFile.h"
#include "Utils.h"

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


    void DownloadsViewModel::startDownloadStatsUpdates() 
    {
        //start the timer here
        timer.Interval(std::chrono::milliseconds{ 300 });
        auto registrationtoken = timer.Tick({ this, &DownloadsViewModel::OnTick });
        timer.Start();
    }

    void DownloadsViewModel::stopDownloadStatsUpdates()
    {
        if (timer != nullptr) {
            timer.Stop();
        }
    }

    void DownloadsViewModel::OnTick(Windows::Foundation::IInspectable const& /* sender */,
        Windows::Foundation::IInspectable const& /* e */)
    {
        for (uint32_t i = 0; i < m_downloadsOBVector.Size(); i++)
        {
            if (m_downloadsOBVector.GetAt(i).IsDownloading())
            {
                updateDownloadStats(i);
            }
            
        }
    }

    void DownloadsViewModel::updateDownloadStats(int index) 
    {
        if (index >= 0 && index < m_downloadsOBVector.Size()) 
        {
            int f_speed = DownloadFile::DownloadInstance().getDownloadspeed(index);
            int f_size = DownloadFile::DownloadInstance().getDownloadedSize(index);
            int f_totalSize = std::stoi(DownloadFile::DownloadInstance().getFileSize(index));
            double d_percentage = (static_cast<double>(f_size) / f_totalSize) * 100;
            if (f_size != f_totalSize) {
                m_downloadsOBVector.GetAt(index).Speed(winrt::to_hstring(Utils::bytesToSize(std::to_string(f_speed))) + L"/s");
                m_downloadsOBVector.GetAt(index).Size(winrt::to_hstring(Utils::bytesToSize(std::to_string(f_size))));
                m_downloadsOBVector.GetAt(index).DPercentage(d_percentage);
                m_downloadsOBVector.GetAt(index).Percentage(winrt::to_hstring(static_cast<int>(d_percentage)) + L"%");
            }
            else {
                m_downloadsOBVector.GetAt(index).Speed(L"Completed");
                m_downloadsOBVector.GetAt(index).Size(winrt::to_hstring(Utils::bytesToSize(std::to_string(f_totalSize))));
                m_downloadsOBVector.GetAt(index).DPercentage(static_cast<double>(100));
                m_downloadsOBVector.GetAt(index).Percentage(winrt::to_hstring(L"100%"));
            }
            
        }
    }
}
