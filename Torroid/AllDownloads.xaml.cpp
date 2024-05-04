#include "pch.h"
#include "AllDownloads.xaml.h"
#if __has_include("AllDownloads.g.cpp")
#include "AllDownloads.g.cpp"
#endif

#include "DownloadFile.h"
#include "logging.h"
#include "DownloadsJson.h"
#include "Utils.h"


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
        DownloadsLV().ItemsSource(MainViewModel().DownloadsOBVector());
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
                        int iResult = DownloadFile::DownloadInstance().addUrl(vUrl);

                        if (iResult == 0 || iResult == 1)
                        {
                            addToDownloadsLV(0);
                        }
                    });
            });
        dialog.ShowAsync();

    }

    void AllDownloads::addToDownloadsLV(int index)
    {
        std::string s_name = DownloadsJson::jsonInstance().filename(index);

        //Logging::Info("Download File Name: " + s_name);
        hstring h_name = to_hstring(Utils::FilePath_to_FileName(s_name));

        std::string s_size = DownloadsJson::jsonInstance().fileSizeTotal(index);
        //Logging::Info("Download File Size: " + s_size);
        hstring h_size = to_hstring(Utils::bytesToSize(s_size));

        MainViewModel().DownloadsOBVector().InsertAt(0, make<Torroid::implementation::Downloads>(h_name, h_size));
        MainViewModel().DownloadsOBVector().GetAt(0).IsDownloading(true);
    }

    void AllDownloads::PlayPauseButton_Clicked(IInspectable const& sender, RoutedEventArgs const& e)
    {
        auto button = sender.as<Microsoft::UI::Xaml::Controls::Button>();
        auto item = button.DataContext().as<Torroid::Downloads>();

        // Update the FontIcon based on the IsDownloading state
        auto fontIcon = button.Content().as<Microsoft::UI::Xaml::Controls::FontIcon>();
        int index = DownloadsJson::jsonInstance().getIndex(to_string(item.FileName()));
        if (item.IsDownloading())
        {
            // Pause the Download
			DownloadFile::DownloadInstance().pause(index);
            // Set FontIcon to play icon when downloading
            fontIcon.Glyph(L"\uF5B0");
        }
        else
        {
            // Resume the Download
            DownloadFile::DownloadInstance().ResumeDownload(index);
            MainViewModel().DownloadsOBVector().GetAt(index).IsDownloading(true);
            // Set FontIcon to pause icon when paused
            fontIcon.Glyph(L"\uF8AE");
        }
    }
}
