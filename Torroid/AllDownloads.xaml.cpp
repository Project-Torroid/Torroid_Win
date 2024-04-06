#include "pch.h"
#include "AllDownloads.xaml.h"
#if __has_include("AllDownloads.g.cpp")
#include "AllDownloads.g.cpp"
#endif

#include "DownloadFile.h"
#include <winrt/Windows.Foundation.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Windows::Foundation::Collections;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::Torroid::implementation
{
    AllDownloads::AllDownloads() {
        InitializeComponent();


        // Set up your ListView
        //BaseExample().ItemsSource(m_viewModel.Downloads());
    }

    void AllDownloads::AdditemtoLV(Windows::Foundation::IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&) 
    {
        MainViewModel().Downloads().FileName(L"To Kill a Mockingbird");
        MainViewModel().DownloadsOBVector().Append(winrt::make<Torroid::implementation::Downloads>(L"Moby Dick.jpg"));
    }

    Torroid::DownloadsViewModel AllDownloads::MainViewModel()
    {
        return m_mainViewModel;
    }

    void AllDownloads::addDownloadBtnClicked(IInspectable const&, RoutedEventArgs const&)
    {
        winrt::Microsoft::UI::Xaml::Controls::ContentDialog dialog;
        dialog.XamlRoot(Content().XamlRoot());
        dialog.Title(winrt::box_value(L"Add Download Link"));
        dialog.PrimaryButtonText(L"Add");
        dialog.CloseButtonText(L"Cancel");
        dialog.DefaultButton(winrt::Microsoft::UI::Xaml::Controls::ContentDialogButton::Primary);

        winrt::Microsoft::UI::Xaml::Controls::TextBox urlBox;
        urlBox.PlaceholderText(L"Enter Direct Download Url");
        dialog.Content(urlBox);

        /*auto appPath = Windows::Storage::ApplicationData::Current().LocalFolder().Path();*/

        dialog.PrimaryButtonClick([this, urlBox = std::move(urlBox)](auto&& ...)
        {
            DispatcherQueue().TryEnqueue([this, urlBox = std::move(urlBox)](auto&& ...)
            {
                
                std::vector<std::string> vUrl = { winrt::to_string(urlBox.Text()) };
                
                // Add Download
                DownloadFile::DownloadInstance().addUrl(vUrl);

            });
        });


        dialog.ShowAsync();

    }
}
