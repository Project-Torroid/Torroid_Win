#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif
#include <winrt/Microsoft.UI.Windowing.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::Torroid::implementation
{
    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }
    MainWindow::MainWindow() {
        this->ExtendsContentIntoTitleBar(true);
        winrt::Microsoft::UI::Windowing::AppWindow appWindow = AppWindow();
        winrt::Microsoft::UI::Windowing::AppWindowTitleBar titlebar = appWindow.TitleBar();
        titlebar.PreferredHeightOption(winrt::Microsoft::UI::Windowing::TitleBarHeightOption::Tall);
    }

    void MainWindow::addDownloadBtnClicked(IInspectable const&, RoutedEventArgs const&)
    {
        winrt::Microsoft::UI::Xaml::Controls::ContentDialog dialog;
        dialog.XamlRoot(Content().XamlRoot());
        dialog.Title(winrt::box_value(L"Add Download Link"));
        dialog.PrimaryButtonText(L"Add");
        dialog.CloseButtonText(L"Cancel");
        dialog.DefaultButton(winrt::Microsoft::UI::Xaml::Controls::ContentDialogButton::Primary);

        dialog.Content();

        dialog.PrimaryButtonClick([&](auto&& ...)
            {
                DispatcherQueue().TryEnqueue([&](auto&& ...)
                   {
                   });
            });


        dialog.ShowAsync();

    }
}
