#include "pch.h"
#include "AllDownloads.xaml.h"
#if __has_include("AllDownloads.g.cpp")
#include "AllDownloads.g.cpp"
#endif

#include <winrt/Windows.Foundation.h>
#include <sstream>

#include "DownloadFile.h"
#include "logging.h"
#include "json.h"


using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Windows::Foundation::Collections;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::Torroid::implementation
{
    AllDownloads::AllDownloads() {
        InitializeComponent();

        std::thread PopulateUi([&]() {

            json jsonFile;

            int v_size = jsonFile.vDownloadEntries.size();

            for (int i = 0; i < v_size; i++)
            {
                AddToDownloadsLV(i, true);
            }
            
        });
        PopulateUi.detach();

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
                    AddToDownloadsLV(0, false);
                }
            });
        });
        dialog.ShowAsync();

    }

    void AllDownloads::AddToDownloadsLV(int index, bool isFirstCall)
    {
        json jsonFile;
        //auto lvi = DownloadsLV().Items().Size();
        auto s_name =jsonFile.vDownloadEntries[index]["filename"];

        //Logging::Info("Download File Name: " + s_name);
        auto h_name = winrt::to_hstring(FilePath_to_FileName(s_name));

        auto s_size = jsonFile.vDownloadEntries[index]["totalFileSize"];
        //Logging::Info("Download File Size: " + s_size);
        auto h_size = winrt::to_hstring(s_size);
        
        if (isFirstCall)
        {
            MainViewModel().DownloadsOBVector().Append(winrt::make<Torroid::implementation::Downloads>(h_name, h_size));
        }
        else MainViewModel().DownloadsOBVector().InsertAt(0,winrt::make<Torroid::implementation::Downloads>(h_name, h_size));
        
    }

    std::string AllDownloads::FilePath_to_FileName(std::string filePath)
    {
        std::string substring;
        std::vector<std::string> substrings; // string vector
        std::istringstream iss(filePath);

        //Seprate file name from full path
        while (std::getline(iss, substring, '/'))
        {
            substrings.push_back(substring);
        }

        // Return file name 
        return substrings.back();
    }
}
