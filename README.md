# Windows File Change Finder

A command-line utility that recursively searches directories to find files modified within a specified number of days. Perfect for tracking recent changes across large directory structures.

## Features

- Recursive directory traversal
- Real-time progress tracking
- Memory-efficient processing
- Detailed search statistics
- Windows-optimized file system operations
- Output results to a dated text file

## Prerequisites

- Windows operating system
- MinGW-w64 compiler
- Git (optional, for cloning the repository)

## Installation

Download the executable file from the Github Releases section and _edit your System Path to include the path_ to the .exe, or

1. Clone the repository or download the source code:
```bash
git clone [repository-url]
cd whatsnew
```

2. Compile the program using MinGW-w64:
```bash
gcc -o whatsnew.exe whatsnew.c
```

## Usage

```bash
whatsnew <folder_path> <days>
```

### Parameters:
- `folder_path`: The directory path to search (e.g., C:\Users\Documents)
- `days`: Number of days to look back for changes (e.g., 7)

### Example:
```bash
whatsnew "C:\Users\John\Documents" 7
```

This will search for all files in the Documents folder (and its subfolders) that were modified in the last 7 days.

## Output

The program creates a file named `changed_files_YYYY-MM-DD.txt` in the current working directory, where `YYYY-MM-DD` is the current date. The output file contains:

- Full path to each changed file
- Filename
- Last modified date and time

Additionally, the program provides console output showing:
- Progress updates (every 1000 files)
- Total number of files checked
- Number of files found within the specified timeframe
- Location of the output file

## Sample Output

Console:
```
Starting search in C:\Users\John\Documents for files changed in last 7 days...
Processed 1000 files...
Processed 2000 files...
Search complete.
Total files checked: 2345
Files changed within 7 days: 15
Results written to changed_files_2024-12-12.txt
```

Output file content:
```
Files Changed Within 7 Days (as of 2024-12-12):

Path: C:\Users\John\Documents\report.docx
Filename: report.docx
Last Modified: 2024-12-11 14:30:22

Path: C:\Users\John\Documents\data\spreadsheet.xlsx
Filename: spreadsheet.xlsx
Last Modified: 2024-12-10 09:15:45
```

## Performance

The utility is optimized for:
- Large directory structures
- Minimal memory usage
- Fast file system traversal
- Windows file system operations

## Limitations

- Windows-specific implementation
- File paths must be less than MAX_PATH characters
- Requires appropriate permissions to access directories

## Contributing

Feel free to submit issues and enhancement requests!

## License

Copyright (C) 2024  Adam Wood

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A  ARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
