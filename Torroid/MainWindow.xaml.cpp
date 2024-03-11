#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Windows.Storage.h>
#include<DownloadFile.h>
#include <SettingsPage.xaml.h>
#include<winrt/Windows.UI.Xaml.Interop.h>
#include <AllDownloads.xaml.h>

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

    void MainWindow::MainWindowNav_SelectionChanged(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs const& args)
    {
        if (args.IsSettingsSelected())
        {
            MainWindowDownloadFilesFrame().Navigate(xaml_typename<Torroid::SettingsPage>());
        }
        else MainWindowDownloadFilesFrame().Navigate(xaml_typename<Torroid::AllDownloads>());

    }
}
