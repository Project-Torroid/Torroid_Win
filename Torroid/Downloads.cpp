#include "pch.h"
#include "Downloads.h"
#include "Downloads.g.cpp"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Data;

namespace winrt::Torroid::implementation
{
    Downloads::Downloads(hstring const& fileName) : m_fileName{ fileName }
    {
    }

    hstring Downloads::FileName()
    {
        return m_fileName;
    }
    void Downloads::FileName(hstring const& value)
    {
        if (m_fileName != value)
        {
            m_fileName = value;
            m_propertyChanged(*this, PropertyChangedEventArgs{ L"FileName" });
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
