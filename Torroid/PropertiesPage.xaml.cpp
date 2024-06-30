#include "pch.h"
#include "PropertiesPage.xaml.h"
#if __has_include("PropertiesPage.g.cpp")
#include "PropertiesPage.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::Torroid::implementation
{
    PropertiesPage::PropertiesPage(hstring fName, hstring fSize)
    {
        InitializeComponent();
        FName().Text(fName);
        FSize().Text(fSize);
        FUrl().Text(L"Dummyurl.com");
    }

    int32_t PropertiesPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void PropertiesPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }
}
