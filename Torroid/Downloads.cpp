#include "pch.h"
#include "Downloads.h"
#include "Downloads.g.cpp"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Data;

namespace winrt::Torroid::implementation
{
    Downloads::Downloads(hstring const& fileName, hstring const& totalSize) : m_fileName{ fileName }, m_totalSize { totalSize }
    {
    }

    hstring Downloads::FileName(){ return m_fileName; }
    void Downloads::FileName(hstring const& value)
    {
        if (m_fileName != value)
        {
            m_fileName = value;
            m_propertyChanged(*this, PropertyChangedEventArgs{ L"FileName" });
        }
    }

    hstring Downloads::Percentage() { return m_percentage; }
    void Downloads::Percentage(hstring const& value)
    {
        if (m_percentage != value)
        {
            m_percentage = value;
            m_propertyChanged(*this, PropertyChangedEventArgs{ L"Percentage" });
        }
    }

    double Downloads::DPercentage(){ return m_dPercentage; }
    void Downloads::DPercentage(double const& value)
    {
        if (m_dPercentage != value)
        {
            m_dPercentage = value;
            m_propertyChanged(*this, PropertyChangedEventArgs{ L"DPercentage" });
        }
    }

    hstring Downloads::CurSize(){ return m_curSize; }
    void Downloads::CurSize(hstring const& value)
    {
        if (m_curSize != value)
        {
            m_curSize = value;
            m_propertyChanged(*this, PropertyChangedEventArgs{ L"CurSize" });
        }
    }

    hstring Downloads::TotalSize(){ return m_totalSize; }
    void Downloads::TotalSize(hstring const& value)
    {
        if (m_totalSize != value)
        {
            m_totalSize = value;
            m_propertyChanged(*this, PropertyChangedEventArgs{ L"TotalSize" });
        }
    }

    hstring Downloads::Speed(){ return m_speed; }
    void Downloads::Speed(hstring const& value)
    {
        if (m_speed != value)
        {
            m_speed = value;
            m_propertyChanged(*this, PropertyChangedEventArgs{ L"Speed" });
        }
    }

    winrt::event_token Downloads::PropertyChanged(PropertyChangedEventHandler const& handler)
    {
        return m_propertyChanged.add(handler);
    }
    void Downloads::PropertyChanged(winrt::event_token const& token) noexcept
    {
        return m_propertyChanged.remove(token);
    }
}
