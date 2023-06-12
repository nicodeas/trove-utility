# trove-utility

Trove is a command-line utility program designed to build and search a compressed index of the contents of files. It aims to provide an efficient way to store and retrieve information from a large number of files based on their content. By indexing the words found in the files, Trove enables quick searches for specific words or phrases within the indexed files.

## Background

With the increasing size and density of computer disks and the decreasing costs of storage, we now store a vast amount of files on our computers. Locating specific information within these files solely based on their names and directories has become challenging. To address this issue, Trove allows users to find information based on the content of the files rather than relying solely on file-system metadata.

Popular operating systems like macOS provide utilities such as Spotlight to index file contents, and there are similar tools available for Linux. Trove aims to offer similar functionality as a command-line utility for Linux and macOS systems.

## Key Features

- Indexes the contents of files to facilitate efficient searching
- Utilizes core Linux and macOS system calls and POSIX function calls

## Usage

### Listing Files with a Word

This lists the absolute pathnames of files containing a specified word.

```
./trove [ -f trovefile ] word
```

### Building, Removing, or Updating Index

This allows for building a new index, removing information from an existing index, or updating an existing index.

```
./trove [-f trovefile] [ -b | -r | -u ] [-l length] filelist
```

The `filelist` is a sequence of one or more filenames or directory names. Trove indexes the contents of each named file and traverses each named directory (including subdirectories) to locate files for indexing.

Note that the files to be indexed can be of any type, including text files, executable programs, or object files.

#### Options

- `-b`: Build a new trove-file from the contents of the `filelist`. The new trove-file will replace an existing trove-file of the same name.
- `-f trovefile`: Specifies the name of the trove-file to be built or searched. If the `-f` option is not provided, the default name of the trove-file is `/tmp/trove`.
- `-l length`: Only words with a length of at least the indicated value will be added to the trove-file. The length must be a positive integer. If the `-l` option is not provided, the default length is 4.
- `-r`: If any of the files from the `filelist` are present in the trove-file, remove all their information from the trove-file.
- `-u` Update the trove-file with the contents of all files in the `filelist`. If any of the files information already exists in the trove-file, the old information is first removed.
