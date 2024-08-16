#include "pch.h"
#include "AllDownloads.xaml.h"
#if __has_include("AllDownloads.g.cpp")
#include "AllDownloads.g.cpp"
#endif

#include "DownloadFile.h"
#include "logging.h"
#include "DownloadsJson.h"
#include "Utils.h"
#include "PropertiesPage.xaml.h"


using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Windows::Foundation;
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

    void AllDownloads::AddDownloadButton_Click(IInspectable const&, RoutedEventArgs const&)
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

    void AllDownloads::PauseResumeButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
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
            // Set FontIcon to Resume icon
            fontIcon.Glyph(L"\uF5B0");
        }
        else
        {
            // Resume the Download
            DownloadFile::DownloadInstance().ResumeDownload(index);
            MainViewModel().DownloadsOBVector().GetAt(index).IsDownloading(true);
            // Set FontIcon to Pause icon
            fontIcon.Glyph(L"\uF8AE");
        }
    }
    void AllDownloads::PropertiesButton_Click(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& e)
    void AllDownloads::PropertiesButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        auto button = sender.as<Controls::MenuFlyoutItem>();
        auto item = button.DataContext().as<Torroid::Downloads>();

        ContentDialog dialog;
        dialog.XamlRoot(Content().XamlRoot());
        dialog.Title(winrt::box_value(L"Properties"));
        dialog.PrimaryButtonText(L"Save");
        dialog.CloseButtonText(L"Cancel");
        dialog.DefaultButton(winrt::Microsoft::UI::Xaml::Controls::ContentDialogButton::Primary);

        Torroid::PropertiesPage dialogContent = winrt::make<Torroid::implementation::PropertiesPage>(item.FileName(), item.Size());
        dialog.Content(dialogContent);

        dialog.ShowAsync();
    }

    void AllDownloads::OpenExplorer_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        auto button = sender.as<Controls::MenuFlyoutItem>();
        auto item = button.DataContext().as<Torroid::Downloads>();

        std::string filePath = DownloadsJson::jsonInstance().filename(Utils::FileName_to_Index(item.FileName()));
        std::string folderPath = Utils::FilePath_to_FolderPath(filePath);

        Windows::System::Launcher::LaunchFolderPathAsync(winrt::to_hstring(folderPath));
    }

    IAsyncAction AllDownloads::OpenFile_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        auto button = sender.as<Controls::MenuFlyoutItem>();
        auto item = button.DataContext().as<Torroid::Downloads>();

        // Get the file path.
        std::string filePath = DownloadsJson::jsonInstance().filename(Utils::FileName_to_Index(item.FileName()));

        // Get the folder path.
        std::string folderPath = Utils::FilePath_to_FolderPath(filePath);

        try
        {
            // Get the folder.
            Windows::Storage::StorageFolder installFolder{ co_await Windows::Storage::StorageFolder::GetFolderFromPathAsync(winrt::to_hstring(folderPath)) };

            // Get the file.
            Windows::Storage::StorageFile file{ co_await installFolder.GetFileAsync(item.FileName()) };

            if (file)
            {
                // Launch the retrieved file.
                bool success{ co_await Windows::System::Launcher::LaunchFileAsync(file) };
                if (success)
                {
                    // File launched.
                }
                else
                {
                    // File launch failed.
                }
            }
            else
            {
                // Couldn't find file.
            }
        }
        catch (winrt::hresult_error const& ex)
        {
            // File launch failed.
            winrt::hresult hr = ex.code();
            winrt::hstring message = ex.message();

            ContentDialog dialog;
            dialog.XamlRoot(Content().XamlRoot());
            dialog.Title(winrt::box_value(L"File launch failed."));
            dialog.CloseButtonText(L"Cancel");
            dialog.DefaultButton(Controls::ContentDialogButton::Close);

            StackPanel stackPanel;
            stackPanel.Orientation(Controls::Orientation::Vertical);

            TextBlock errorCode;
            errorCode.Text(L"Error Code: " + winrt::to_hstring(hr));
            errorCode.TextWrapping(TextWrapping::Wrap);

            TextBlock errorMessage;
            errorMessage.Text(message);
            errorMessage.TextWrapping(TextWrapping::Wrap);

            stackPanel.Children().Append(errorCode);
            stackPanel.Children().Append(errorMessage);
            dialog.Content(stackPanel);

            dialog.ShowAsync();
        }
    }
    IAsyncAction AllDownloads::RemoveDownloadButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
		auto button = sender.as<Controls::Button>();
		auto item = button.DataContext().as<Torroid::Downloads>();

        removeDownloadContentDialog().XamlRoot(Content().XamlRoot());
        Controls::ContentDialogResult result{ co_await removeDownloadContentDialog().ShowAsync() };

        if(result == Controls::ContentDialogResult::Primary)
		{
			// Remove the item from the ListView
			int index = Utils::FileName_to_Index(item.FileName());
			if(item.IsDownloading())
			{
				DownloadFile::DownloadInstance().pause(index);
			}
            DownloadFile::DownloadInstance().removeDownload(index, ConfirmFileDeletionCheckBox().IsChecked().GetBoolean());
			MainViewModel().DownloadsOBVector().RemoveAt(index);
		}
    }
}
