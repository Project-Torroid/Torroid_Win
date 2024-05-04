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

        for (size_t index = 0; index < DownloadsJson::jsonInstance().size(); index++)
        {
            std::string f_name = Utils::FilePath_to_FileName(DownloadsJson::jsonInstance().filename(index));
            std::string f_sizeTotal = Utils::bytesToSize(DownloadsJson::jsonInstance().fileSizeTotal(index));

            DownloadsOBVector().Append(make<Torroid::implementation::Downloads>(to_hstring(f_name), to_hstring(f_sizeTotal)));
			DownloadsOBVector().GetAt(index).IsDownloading(true);
        }
		startDownloadStatsUpdates();
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
            std::string f_status = DownloadsJson::jsonInstance().fileStatus(index);
            if (f_status == "Downloading")
            {
                int64_t f_sizeCurrent = DownloadFile::DownloadInstance().getCurrentFileSize(index);
                int f_speed = DownloadFile::DownloadInstance().getDownloadspeed(index);
                int64_t f_sizeTotal = DownloadFile::DownloadInstance().getTotalFileSize(index);
                double d_percentage = ( (double)f_sizeCurrent / (double)f_sizeTotal ) * 100 ;

                m_downloadsOBVector.GetAt(index).Speed(winrt::to_hstring(Utils::bytesToSize(std::to_string(f_speed))) + L"/s");
                m_downloadsOBVector.GetAt(index).Size(winrt::to_hstring(Utils::bytesToSize(std::to_string(f_sizeCurrent))));
                m_downloadsOBVector.GetAt(index).DPercentage(d_percentage);

                std::string formatted_percentage = std::to_string(d_percentage);
                size_t pos = formatted_percentage.find(".");
                if (pos != std::string::npos && formatted_percentage.size() > pos + 3) {
                    formatted_percentage = formatted_percentage.substr(0, pos + 3); // Cut off after two decimal places
                }

                m_downloadsOBVector.GetAt(index).Percentage( winrt::to_hstring(formatted_percentage) + L"%");
				m_downloadsOBVector.GetAt(index).IsDownloading(true);
        
			}
            else if (f_status != "Downloading")
            {
                std::string f_sizeCurrent = DownloadsJson::jsonInstance().fileSizeCurrent(index);
                std::string f_sizeTotal = DownloadsJson::jsonInstance().fileSizeTotal(index);
                std::string d_percentage = DownloadsJson::jsonInstance().fileDownloadPercentage(index);
                m_downloadsOBVector.GetAt(index).Size(winrt::to_hstring(Utils::bytesToSize(f_sizeCurrent)));
                m_downloadsOBVector.GetAt(index).DPercentage(static_cast<double>(std::stoi(d_percentage)));
                m_downloadsOBVector.GetAt(index).Percentage(winrt::to_hstring(d_percentage) + L"%");
                m_downloadsOBVector.GetAt(index).IsDownloading(false);
				m_downloadsOBVector.GetAt(index).Speed(winrt::to_hstring(f_status));
            }
        }
    }
}
