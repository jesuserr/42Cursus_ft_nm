# ft_nm - 42 School Unix Project

A complete reimplementation in C of the Unix `nm` command that lists symbols from ELF binary files. This project demonstrates binary file parsing, memory management, and systems programming concepts.

[![42 School](https://img.shields.io/badge/42-School-000000?style=flat&logo=42&logoColor=white)](https://42.fr)
[![Language](https://img.shields.io/badge/Language-C-blue)](https://en.wikipedia.org/wiki/C_(programming_language))
![Unix](https://img.shields.io/badge/Platform-Unix/Linux-green)

## 📋 Subject Requirements

Based on the official 42 School subject (Version 3.1):

**Program name**: `ft_nm`  
**Authorized External functions**: open, close, mmap, munmap, write, fstat, malloc, free, exit, perror, strerror  

### Technical Requirements
- Work with ELF binaries on Linux (x86_32, x64 architectures)
- Support object files (.o), shared libraries (.so), and executables
- Output must match system `nm` (order, offset, padding)*
- Robust error handling - no crashes (segfaults, bus errors, double free)
- Use of custom libft library with proper Makefile integration

*\*Note: Symbol ordering uses custom `ft_strcmp` implementation - set `LC_ALL=C` for exact system `nm` output matching*

## ✨ Features

### Mandatory Features (Required)
- **Basic nm functionality**: Symbol listing without options
- **Multiple files**: Process multiple files in sequence
- **Default file**: Use `a.out` when no file specified
- **Exit status**: System `nm` compatible exit codes

### Bonus Features (Options: -a, -g, -p, -r, -u)
- **`-a`**: Display all symbols including debugging symbols
- **`-g`**: Show only external/global symbols  
- **`-p`**: Display symbols in original order (no sorting)
- **`-r`**: Reverse alphabetical ordering
- **`-u`**: Show only undefined symbols

### Additional Features (-j, -S)
- **`-j`**: Display only symbol names (no addresses/types)
- **`-S`**: Print size of defined symbols

## 🚀 Installation & Usage

```bash
# Clone and build
git clone https://github.com/jesuserr/42Cursus_ft_nm.git
cd 42Cursus_ft_nm
git submodule update --init --recursive
make

# Basic usage
./ft_nm [option(s)] [file(s)]

# Examples
./ft_nm /bin/ls                    # List symbols
./ft_nm -g program                 # Global symbols only
./ft_nm -ur /bin/ls                # Undefined symbols, reverse order
./ft_nm file1.o file2.o            # Multiple files
```

### Make Targets
```bash
make          # Build project
make clean    # Remove object files  
make fclean   # Remove all generated files
make re       # Clean rebuild
```

## 📊 Command Line Options

| Option | Description | Type |
|--------|-------------|------|
| (none) | List all symbols in default format | Mandatory |
| `-a` | Display debugger symbols | Bonus |
| `-g` | Display only external symbols | Bonus |
| `-p` | No sorting (preserve order) | Bonus |
| `-r` | Reverse sorting order | Bonus |
| `-u` | Display only undefined symbols | Bonus |
| `-j` | Symbol names only | Additional Bonus |
| `-S` | Print size of defined symbols | Additional Bonus |
| `--help` | Display program options and exit | Additional Bonus |

### Exit Status Codes
- **0** : Success - all files processed successfully
- **>0** : Error - one or more files failed to process

## 🔧 Technical Implementation

### Core Components
1. **ELF Parser**: Validates and extracts ELF headers (32/64-bit)
2. **Symbol Extractor**: Parses symbol tables with filtering
3. **Merge Sort**: Custom sorting with reverse support
4. **Output Formatter**: System `nm` compatible formatting
5. **Memory Manager**: `mmap`-based efficient file access

### Key Data Structures
```c
typedef struct s_args {
    bool        debugger_only;      // option -a
    bool        external_only;      // option -g
    bool        just_symbols;       // option -j (extra bonus)
    bool        no_sort;            // option -p
    bool        reverse_sort;       // option -r
    bool        print_size;         // option -S (extra bonus)
    bool        undefined_only;     // option -u
    t_list      *cli_files_list;    // linked list of files to 'nm'
    void        *file_content;      // pointer to the mapped file content
    char        *file_name;         // name of the current file
    uint64_t    file_size;          // size of the current file
    uint64_t    exit_status;        // exit status of the program
}   t_args;

typedef struct s_data {
    Elf32_Ehdr  elf32_header;       // ELF 32-bit header
    Elf32_Shdr  *elf32_sec_table;   // 32-bit section header table
    Elf32_Sym   *elf32_sym_table;   // 32-bit symbol table
    Elf64_Ehdr  elf64_header;       // ELF 64-bit header
    Elf64_Shdr  *elf64_sec_table;   // 64-bit section header table
    Elf64_Sym   *elf64_sym_table;   // 64-bit symbol table
    char        *str_table;         // String table
    char        *shstr_table;       // Section header string table
    uint64_t    sym_table_ix;       // Symbol table index
    t_list      *sym_list;          // Linked list of symbols
    uint8_t     elf_class;          // ELF class (32/64-bit)
}   t_data;
```

### Project Structure
```
ft_nm/
├── Makefile
├── README.md
├── libft/                 # Custom C library
└── srcs/
    ├── ft_nm.h            # Header file
    ├── main.c             # Main program
    ├── parser.c           # Argument parsing
    ├── headers.c          # ELF processing
    ├── print_symbols.c    # Output formatting
    ├── print_utils.c      # Utilities
    └── merge_sort.c       # Sorting
```

## 📝 Symbol Types

| Symbol | Meaning |
|--------|---------|
| `A`/`a` | Absolute symbol (global/local) |
| `B`/`b` | BSS section (uninitialized data) |
| `C` | Common symbol |
| `D`/`d` | Data section (initialized data) |
| `i` | Indirect function (GNU IFUNC) |
| `N`/`n` | Debug symbol |
| `R`/`r` | Read-only data section |
| `T`/`t` | Text section (code) |
| `U` | Undefined symbol |
| `V`/`v` | Weak object |
| `W`/`w` | Weak symbol |

*Uppercase = global, lowercase = local*

## 🧪 Testing

### Basic Testing
```bash
# Mandatory requirements
./ft_nm /bin/ls                           # Basic functionality
./ft_nm /bin/ls /bin/cat                  # Multiple files
./ft_nm                                   # Default file (a.out)
./ft_nm nonexistent; echo "Exit: $?"      # Error handling

# Bonus features
./ft_nm -a /bin/ls                        # All symbols
./ft_nm -g /bin/ls                        # Global only
./ft_nm -u /bin/ls                        # Undefined only
./ft_nm -agpr /bin/ls                     # Combined options

# Additional features
./ft_nm -j /bin/ls                        # Names only
./ft_nm -S /bin/ls                        # Print size
```

### Comparison Testing
```bash
# Compare with system nm
nm /bin/ls > system.txt
./ft_nm /bin/ls > mine.txt
diff system.txt mine.txt

# Memory testing
valgrind --leak-check=full ./ft_nm /bin/ls
```

## 📋 Evaluation Checklist

### Mandatory (Must Pass)
- [ ] Compiles without warnings (`-Wall -Wextra -Werror`)
- [ ] Handles 32-bit and 64-bit ELF files
- [ ] Processes multiple files correctly
- [ ] Error handling for invalid files
- [ ] Default file behavior (`a.out`)
- [ ] Output matches system `nm` exactly
- [ ] Correct exit status codes
- [ ] No memory leaks

### Bonus (Extra Credit)
- [ ] All `-a`, `-g`, `-p`, `-r`, `-u` options work
- [ ] Options can be combined
- [ ] `-j` and `-S` additional features
- [ ] Bonus doesn't break mandatory functionality

### Code Quality
- [ ] Proper error handling
- [ ] Clean, modular code

## 🙏 Acknowledgments

### Technical References
- **ELF Specification**: [Tool Interface Standard (TIS) ELF Specification](https://refspecs.linuxfoundation.org/elf/elf.pdf)
- **ELF Format Overview**: [Executable and Linkable Format - Wikipedia](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format)

### Educational Resources
- **Learning Linux Binary Analysis** by Ryan O'Neill: [O'Reilly Media](https://www.oreilly.com/library/view/learning-linux-binary/9781782167105/)
- **Practical Binary Analysis** by Dennis Andriesse: [No Starch Press](https://nostarch.com/binaryanalysis#content)

### Video Tutorials
- [Understanding ELF Files](https://www.youtube.com/watch?v=i1UDF05iZPU) - Comprehensive ELF format explanation
- [Binary Analysis Fundamentals](https://www.youtube.com/watch?v=L9okXJH5l2Y) - Binary file analysis techniques  
- [ELF File Structure Deep Dive](https://www.youtube.com/watch?v=nC1U1LJQL8o) - Advanced ELF internals

### Testing Tools
- **nm_tester** by jemercie: [GitHub Repository](https://github.com/jemercie/nm_tester) - Comprehensive testing suite for ft_nm projects

## 🎯 Key Achievements

- ✅ **Full ELF Support**: Both 32-bit and 64-bit architectures
- ✅ **Memory Efficient**: `mmap`-based file access
- ✅ **Robust**: Comprehensive error handling and validation
- ✅ **Feature Complete**: All mandatory + bonus + additional features
- ✅ **Production Quality**: Matches system `nm` output exactly

---

*This project demonstrates mastery of binary file formats, memory management, and Unix tool implementation as part of the 42 School systems programming curriculum.*
