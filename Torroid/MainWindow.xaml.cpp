#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include "SettingsPage.xaml.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Media::Animation;

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
        this->InitializeComponent();
        this->ExtendsContentIntoTitleBar(true);
        winrt::Microsoft::UI::Windowing::AppWindow appWindow = AppWindow();
        winrt::Microsoft::UI::Windowing::AppWindowTitleBar titlebar = appWindow.TitleBar();
        titlebar.PreferredHeightOption(winrt::Microsoft::UI::Windowing::TitleBarHeightOption::Standard);

        // default selected item for Navigation
        auto defaultNavSelection = MainWindowNav().MenuItems().GetAt(0).as<Controls::NavigationViewItem>();
        MainWindowNav().SelectedItem(defaultNavSelection);
    }

    void MainWindow::MainWindowNav_SelectionChanged(
        Controls::NavigationView const& sender, 
        Controls::NavigationViewSelectionChangedEventArgs const& args)
    {
        // Create the slide transition and set the transition effect to FromLeft.
        SlideNavigationTransitionInfo slideEffect = SlideNavigationTransitionInfo();
        slideEffect.Effect(SlideNavigationTransitionEffect(SlideNavigationTransitionEffect::FromLeft));

        if (args.IsSettingsSelected())
        {
            MainWindowDownloadFilesFrame().Navigate(xaml_typename<Torroid::SettingsPage>(), nullptr, slideEffect);
        }
        else {

            /* Navigate to AllDownloads for now
            * TODO: Navigate to nav item's respective page when added as initial work is alredy done*/
            auto selectedItem = args.SelectedItem().as<Controls::NavigationViewItem>();
            winrt::hstring selectedItemTag = unbox_value<winrt::hstring>(selectedItem.Tag());
            winrt::hstring pageName = L"Torroid.AllDownloads";
            auto pageType = xaml_typename<Controls::Frame>();
            pageType.Name = pageName;
            MainWindowDownloadFilesFrame().Navigate(pageType, nullptr, slideEffect);
        }

    }
}
