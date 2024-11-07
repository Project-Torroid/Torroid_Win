
# ![Torroid](Torroid/Assets/Square44x44Logo.altform-lightunplated_targetsize-48.png) Torroid Download Manager

Torroid is a Windows application for downloading files from the internet. It is built using **C++/WinRT** and **WinUI 3** for the user interface, and it utilizes **Aria2** for the actual file downloading process.

## Features

- **Modern UI**: Built with WinUI 3 for a smooth and responsive user experience.
- **High Performance Downloads**: Leverages Aria2 for efficient and reliable file downloading.
- **Multi-protocol Support**: Supports HTTP, HTTPS, FTP, and BitTorrent protocols via Aria2.
- **Pause/Resume Downloads**: Ability to pause and resume downloads.
- **Concurrent Downloads**: Supports multiple downloads simultaneously.
- **Download Management**: Simple UI to manage ongoing, paused, and completed downloads.

## Getting Started

### Prerequisites

- **Windows 10 (Version 1809 or higher)**
- **Visual Studio 2022** with the following components:
  - Universal Windows Platform development
  - C++/WinRT
  - .NET Desktop Development

### Installation

1. Clone the repository:

    ```shell
    git clone --recurse-submodules https://github.com/Project-Torroid/Torroid_Win.git
    ```

2. Open the solution file (`Torroid.sln`) in Visual Studio 2022.

3. Restore NuGet packages if required.

4. Build the solution.

5. Run the application.

### Usage

1. **Start a New Download**: Click on the "New Download" button, enter the URL of the file, and choose the destination folder.
2. **Manage Downloads**: Use the download manager to pause, resume, or cancel downloads.
3. **Settings**: Configure Aria2 settings such as the number of concurrent downloads, download speed limits, and more.


## Contributing

Contributions are welcome! If you'd like to contribute, please fork the repository and create a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- **Mile.Aria2**: A lightweight multi-protocol & multi-source, cross-platform download utility.
- **Microsoft WinUI Team**: For the modern UI framework.

