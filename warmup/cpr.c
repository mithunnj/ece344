// Owner: Mithun Jothiravi
// Course : ECE 344

/* Decription: Now you will be writing a program that will make a copy of a directory and all its files and subdirectories. This program, which we will call cpr (copy recursive), will use several file-system related system calls, providing you experience with programmatically using the file system functionality of the operating system. Later in the course, we will study how a file system is designed and implemented, and you will hopefully find this experience beneficial.

To implement the cpr program, you will need to use several file and directory related system calls (and some library functions). Your cpr program has two main parts, as described below: 1) copy a single file, 2) copy all the files in a directory, and then recursively copy all sub-directories.

Copy a Single File
In Unix, before reading or writing a file, it is necessary to tell the operating system of your intent to do so, a process called opening a file. The system then checks whether you have the right to read or write the file, and if so, it returns a non-negative integer called the file descriptor. Then the file is read or written using this descriptor. Finally, when all reads and writes to the file are done, the file descriptor is closed.

To see the manual for the open system call, type:

$ man 2 open
Alternatively, you can type "linux man 2 open" in a browser. The "2" stands for a system call. As described in the manual, you will need to include the following files in your source code to use the open system call:

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
Then a file can be opened as shown below. The pathname argument is the entire path of the file (e.g., /a/b/filename). The flags argument specifies whether a file is opened for reading or writing. The open system call returns the file descriptor fd.

  int fd, flags;
  fd = open(pathname, flags);
The descriptor value is negative if there is an error. More generally, in Unix, all system calls return a negative value on error. You should always check the return value of a system call, and if the return value is negative, perform some type of error handling. For our lab, you should invoke the syserror() macro defined in the code that we have provided.

After a file is opened, it is read using the file descriptor fd as shown below (note that a read will fail if the fd is negative since the open had failed previously). This read system call reads the first 4096 bytes of the file and places the data in the buf buffer, and returns the number of bytes that were read. A second call to read will then read the next 4096 bytes of the file. When a read system call has read all the bytes in the file, its return value is 0, indicating that the end of file has been reached.

  char buf[4096];
  int ret;
  ret = read(fd, buf, 4096);
To copy a file, you will need the following system calls: open, creat, read, write, and close. You will need to carefully read the manual pages for these system calls to see how they need to be invoked to copy a file.

To check if your file copy function is working correctly, you can use the diff program to check whether the source and destination files are the same.

Copy a Directory
Copying a directory involves copying all the files in the directory. To do so, you need to invoke directory-related system calls. In Unix, a directory contains a set of directory entries (also called dirent structures). A directory entry refers to a file (or a sub-directory) and contains the name of the file and a number (called inode number) that uniquely identifies the file.

Before reading a directory, it needs to be opened using the opendir call, similar to opening a file. Then the directory entries can be read one-by-one by using the readdir call. Finally, when all directory entries have been read, the directory is closed by using the closedir call. These three calls are library routines that are wrappers around lower-level system calls.

To create a directory, you need to use the mkdir system call.

Previously, we mentioned that a directory entry structure refers to either a file or a sub-directory. For a file, we need to copy the file, while for a directory, we need to copy it recursively. The directory entry structure does not tell us whether it refers to a file or directory. To find out, you need to use the stat system call. This call takes a pathname and returns a stat structure that provides various types of information about the file or directory such as its type (file, directory, etc.), permissions, ownership, etc.

The type and the permissions of the file are encoded in the st_mode field of the stat structure. You will need to use the S_ISREG and S_ISDIR macros on this field to determine the type of the file/directory (see the man page of stat). You can ignore all other types (e.g., symbolic links, etc.). You will also need to extract the permissions of the source file or directory from the st_mode field and use the chmod system call to set the same permissions on the target file or directory.

To copy a directory, you will need the following library calls: opendir, readdir and closedir, and the following system calls: mkdir, stat and chmod. For the library calls, such as opendir, you can read its manual pages with the command man 3 opendir.

You need to write your cpr program in the file cpr.c. To test, whether your program works correctly, run test_cpr. It should output "Ok" three times.
 */
#include "common.h"
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>

// Bool definitions
#define TRUE 1
#define FALSE 0

#define MAX_FILENAME 300

int PROGRAM_COUNTER = 0; // Store information on the number of recursive folder copy calls.

int is_dir(char *src_path)
{
	struct stat statbuf; // Buffer to store the stat system call information

	// Get information about the filepath from the stat system call w/ error checking
	int stat_ret = stat(src_path, &statbuf);
	
	if (stat_ret < 0) {
		syserror(stat, __FILE__);
	}

	if (S_ISDIR(statbuf.st_mode)) {
		return TRUE;
	} else {
		return FALSE;
	}
}

mode_t mode(char *src_path) {
	struct stat statbuf; // Buffer to store the stat system call information

	// Get information about the filepath from the stat system call w/ error checking
	int stat_ret = stat(src_path, &statbuf);
	
	if (stat_ret < 0) {
		syserror(stat, __FILE__);
	}

	return statbuf.st_mode;
}

void close_dir(DIR *folder) {
	int close_ret = closedir(folder); 
	if (close_ret < 0) {
		syserror(closedir, __FILE__);
	}

	return;
}

void copy_file(char *srcdir, char *dstdir) {

	char buf[4096]; // Buffer to store contents of file

	// Open source directory
	int src_fd = open(srcdir, O_RDONLY);
	// Error check open command
	if (src_fd < 0) {
		syserror(open, __FILE__);
	}

	// Open dest directory, create directory if directory doesn't exist
	int dst_fd = open(dstdir, O_CREAT|O_WRONLY|O_APPEND, 0777); // Set r,w,x permissions to the destination file
	if (dst_fd < 0) {
		syserror(open, __FILE__);
	} 

	while (1) {
		int read_ret = read(src_fd, buf, 4096); // Read contents of the file

		// Perform error checking
		if (read_ret < 0) {
			syserror(read, __FILE__);
		} else if (read_ret == 0) { // If there was no bytes read, then end of file has been reached - break out of loop
			break;
		} else {
			// Write the contents to the new directory
			int write_ret = write(dst_fd, buf, read_ret); // Read # of bytes specified by read_ret

			// Peform error checking
			if (write_ret < 0) {
				syserror(write, __FILE__);
			}
		}
	
	}

	// Change the permissions to the appropriate type, according to the source
	int chmod_ret = chmod(dstdir, mode(srcdir));
	if (chmod_ret < 0) {
		syserror(chmod, __FILE__);
	}

	// Close connection to files
	int src_close = close(src_fd);
	if (src_close < 0) {
		syserror(close, __FILE__);
	}

	int dst_close = close(dst_fd);
	if (dst_close < 0) {
		syserror(close, __FILE__);
	}

	return;
}

void folder_copy(char *srcdir, char *dstdir) {

	// Check if parent folder exists (specified by PROGRAM_COUNTER == 1)
	PROGRAM_COUNTER++;
	if (PROGRAM_COUNTER == 1)  {
		int check_dir = mkdir(dstdir, mode(srcdir));
		if (check_dir == -1) { // If the parent folder already exists spit out an error
			fprintf(stderr, "mkdir: %s: File exists", dstdir);
			exit(1); // Exit with error
		}
	}

	DIR *folder = opendir(srcdir); // Open the folder w/ error checking
	char update_src[MAX_FILENAME];  // Temp placeholder to hold filepath in the loop 
	char update_dst[MAX_FILENAME];  // Temp placeholder to hold filepath in the loop 

	if (folder == NULL) {
		syserror(opendir, __FILE__);
	}

	// Iterate through the files & folders in directory
	struct dirent *directory = readdir(folder);
	while (directory != NULL) {

		// Ignore all mention of current directory ".", and parent directory ".."
		if ( (strcmp(directory->d_name, ".") != 0) && (strcmp(directory->d_name, "..") != 0) )  {

			// Check if the current filepath is a file or a folder
			sprintf(update_src, "%s/%s", srcdir, directory->d_name);
			sprintf(update_dst, "%s/%s", dstdir, directory->d_name);

			if (is_dir(update_src)) {

				// Create folder in the destination directory
				int mkdir_ret = mkdir(update_dst, 0777);

				// return 0 - directory was created, return -1, the directory already exists
				if ( (mkdir_ret == 0) || (mkdir_ret == -1) ){ // If the folder is setup, recursively call this function.
					folder_copy(update_src, update_dst);
				} else {
					syserror(mkdir, __FILE__);
				}

			} else { // If the current filepath is a file, just copy it to the destination
				copy_file(update_src, update_dst);
			}
		}
		directory = readdir(folder); // Move to the next file
		
		}

		// Change the permissions to the appropriate type, according to the source
		int chmod_ret = chmod(dstdir, mode(srcdir));
		if (chmod_ret < 0) {
			syserror(chmod, __FILE__);
		}

		// Close directory w/ error checking
		close_dir(folder);

	return;
}

int main(int argc, char *argv[])
{
	// Check that user passed in arguments
	if (argv[1] == NULL || argv[2] == NULL) {
		return 1;
	}

	if (is_dir(argv[1])) { // If srcdir is a folder, do recursive copy
		folder_copy(argv[1], argv[2]);
	} else { // If srcdir is a file, do a file copy
		copy_file(argv[1], argv[2]); 
	}

	return 0;
}
