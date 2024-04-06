#pragma once
#include "Downloads.g.h"

namespace winrt::Torroid::implementation
{
    struct Downloads : DownloadsT<Downloads>
    {
        Downloads() = delete;

        Downloads(hstring const& fileName);
        hstring FileName();
        void FileName(hstring const& value);
        winrt::event_token PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;

    private:
        winrt::hstring m_fileName;
        winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
    };
}
namespace winrt::Torroid::factory_implementation
{
    struct Downloads : DownloadsT<Downloads, implementation::Downloads>
    {
    };
}
