#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

// Linked list to store file paths dynamically
struct FileNode {
    char path[MAX_PATH];
    char filename[256];
    time_t modified_time;
    struct FileNode* next;
};

// Global variables
int max_days;
FILE* output_file;
time_t current_time;
unsigned long total_files_checked = 0;
unsigned long files_found = 0;

// Function to add file to linked list
void add_file_to_list(struct FileNode** head, const char* path, const char* filename, time_t modified_time) {
    struct FileNode* new_node = malloc(sizeof(struct FileNode));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    strncpy(new_node->path, path, MAX_PATH - 1);
    strncpy(new_node->filename, filename, 255);
    new_node->modified_time = modified_time;
    new_node->next = *head;
    *head = new_node;
}

// Convert Windows FILETIME to time_t
time_t filetime_to_timet(FILETIME ft) {
    ULARGE_INTEGER ull;
    ull.LowPart = ft.dwLowDateTime;
    ull.HighPart = ft.dwHighDateTime;
    return (time_t)(ull.QuadPart / 10000000ULL - 11644473600ULL);
}

// Recursive directory search function
void search_directory(const char* dir_path, struct FileNode** file_list) {
    WIN32_FIND_DATA find_data;
    HANDLE find_handle;
    char search_path[MAX_PATH];
    char full_path[MAX_PATH];

    // Construct the search path
    snprintf(search_path, sizeof(search_path), "%s\\*", dir_path);

    // Start finding files
    find_handle = FindFirstFile(search_path, &find_data);
    if (find_handle == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Cannot open directory: %s\n", dir_path);
        return;
    }

    do {
        // Skip current and parent directory
        if (strcmp(find_data.cFileName, ".") == 0 || strcmp(find_data.cFileName, "..") == 0) {
            continue;
        }

        // Construct full path
        snprintf(full_path, sizeof(full_path), "%s\\%s", dir_path, find_data.cFileName);

        total_files_checked++;

        // Progress update
        if (total_files_checked % 1000 == 0) {
            printf("Processed %lu files...\n", total_files_checked);
        }

        // If it's a directory, recursively search
        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            search_directory(full_path, file_list);
        }
        // If it's a regular file, check modification time
        else {
            time_t file_time = filetime_to_timet(find_data.ftLastWriteTime);
            double days_diff = difftime(current_time, file_time) / (24 * 60 * 60);
            
            if (days_diff <= max_days) {
                add_file_to_list(file_list, full_path, find_data.cFileName, file_time);
                files_found++;
            }
        }
    } while (FindNextFile(find_handle, &find_data));

    FindClose(find_handle);
}

// Write results to file
void write_results(struct FileNode* file_list) {
    struct FileNode* current = file_list;
    struct FileNode* temp;

    while (current != NULL) {
        char modified_time[64];
        struct tm* timeinfo = localtime(&(current->modified_time));
        strftime(modified_time, sizeof(modified_time), "%Y-%m-%d %H:%M:%S", timeinfo);

        fprintf(output_file, "Path: %s\nFilename: %s\nLast Modified: %s\n\n", 
                current->path, current->filename, modified_time);

        // Free memory
        temp = current;
        current = current->next;
        free(temp);
    }
}

int main(int argc, char *argv[]) {
    // Check for correct number of arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <folder_path> <days>\n", argv[0]);
        return 1;
    }

    // Parse arguments
    const char* search_path = argv[1];
    max_days = atoi(argv[2]);

    // Get current time
    current_time = time(NULL);

    // Create output filename with current date
    char output_filename[256];
    struct tm* timeinfo = localtime(&current_time);
    char date_str[20];
    strftime(date_str, sizeof(date_str), "%Y-%m-%d", timeinfo);
    snprintf(output_filename, sizeof(output_filename), "changed_files_%s.txt", date_str);

    // Open output file
    output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        return 1;
    }

    // Write header to output file
    fprintf(output_file, "Files Changed Within %d Days (as of %s):\n\n", 
            max_days, date_str);

    // Initialize file list
    struct FileNode* file_list = NULL;

    // Start searching
    printf("Starting search in %s for files changed in last %d days...\n", 
           search_path, max_days);

    // Perform recursive search
    search_directory(search_path, &file_list);

    // Write results to file
    write_results(file_list);

    // Close output file
    fclose(output_file);

    // Print final summary
    printf("Search complete.\n");
    printf("Total files checked: %lu\n", total_files_checked);
    printf("Files changed within %d days: %lu\n", max_days, files_found);
    printf("Results written to %s\n", output_filename);

    return 0;
}