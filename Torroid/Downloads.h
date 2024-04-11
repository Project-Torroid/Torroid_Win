#pragma once
#include "Downloads.g.h"
#include "DownloadFile.h"

namespace winrt::Torroid::implementation
{
    struct Downloads : DownloadsT<Downloads>
    {
        Downloads() = delete;
        Downloads(hstring const& fileName, hstring const& totalSize);

        hstring FileName();
        void FileName(hstring const& value);

        hstring Percentage();
        void Percentage(hstring const& value);
        
        double DPercentage();
        void DPercentage(double const& value);

        hstring Size();
        void Size(hstring const& value);

        hstring Speed();
        void Speed(hstring const& value);

        winrt::event_token PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;

    private:
        hstring m_fileName;
        hstring m_percentage = L"0%";
        double m_dPercentage = 0;
        hstring m_totalSize = L"0 MB";
        hstring m_size = L"Size: 0 Mb / " + m_totalSize;
        hstring m_speed = L"0 Mbps";
        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
    };
}
namespace winrt::Torroid::factory_implementation
{
    struct Downloads : DownloadsT<Downloads, implementation::Downloads>
    {
    };
}
