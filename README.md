# lfcaps

`lfcaps` is a small C library and command-line tool for reading, writing, inspecting, and modifying Linux file capabilities. It provides a user-level representation of permitted and inheritable capability sets and translates them to and from the Linux extended attribute format used by the kernel.

The project is intended for Linux programs and shell workflows that need to inspect or manage file capabilities without dealing directly with the kernel capability xattr layout.

<b>This readme is written by ai, everything else manually.</b>

## Features

- Read file capabilities by path or from an open file descriptor.
- Write permitted and inheritable capability sets by path or file descriptor.
- Support the Linux file capability formats reported as versions 1, 2, and 3.
- Convert capability bitsets to readable names such as `chown` and `net_bind_service`.
- Convert capability names back to a capability bitset.
- Provide a standalone `lfcaps` command-line tool.

## Command-line tool

The standalone tool lists capabilities by default:

```text
Usage: lfcaps [-vhulLasdciptN] [-n capnames] [file] [file2] ..

 -h                help
 -u                show usage
 -v                verbose
 -l                list caps (default)
 -L                list caps, if present
 -a                add caps
 -s                set caps
 -d                delete caps
 -c                clear all caps
 -i                modify inheritable capset
 -p                modify permitted capset (default)
 -t                test for capabilities
 -n capnames       capset, caps separated by ','
 -N                list cap names
```

Examples:

```sh
# List capabilities on a file
lfcaps /path/to/program

# List only files that have capabilities
lfcaps -L /path/to/program

# Set permitted capabilities
lfcaps -s -n cap_net_bind_service /path/to/program

# Add a capability to the inheritable set
lfcaps -a -i -n cap_net_raw,cap_chroot /path/to/program

# Test whether a file contains a capability
lfcaps -t -n cap_net_bind_service /path/to/program
```

Changing file capabilities normally requires appropriate privileges, such as `CAP_SETFCAP`, and is subject to the Linux filesystem and user-namespace rules.

## C API

Include the public header:

```c
#include "lfcaps.h"
```

The public API uses the following data type:

```c
typedef struct _lfcaps_t {
    uint64_t permitted;
    uint64_t inheritable;
    uint32_t rootid;
    char version;
} lfcaps_t;
```

`permitted` and `inheritable` are capability bitsets. The project defines capability constants such as `LFCAP_CHOWN`, `LFCAP_SETUID`, and `LFCAP_NET_BIND_SERVICE`. `rootid` is used by Linux file capability version 3, and `version` contains the version detected when reading a file.

### `lfcaps_read`

```c
int lfcaps_read(lfcaps_t *caps, const char *path);
```

Reads the capabilities attached to `path` and stores the decoded result in `*caps`.

Return values:

- `0`: the file has no capabilities; the output structure is cleared.
- `1`, `2`, or `3`: the file capabilities were read successfully and the return value is the detected file capability version.
- A negative error value: an error occurred. The value is the negated `errno` value, or `-EINVAL` if the xattr size does not match the capability version.

Example:

```c
lfcaps_t caps;
int result = lfcaps_read(&caps, "/path/to/program");
if (result < 0) {
    /* result is -errno */
} else if (result == 0) {
    /* no file capabilities */
}
```

### `lfcaps_readfd`

```c
int lfcaps_readfd(lfcaps_t *caps, int fd);
```

Reads capabilities from an already open file descriptor. This variant is useful when the caller has already opened the file or wants to avoid resolving the file by path.

The return values are the same as for `lfcaps_read`. The file descriptor must be valid and open for an object on a filesystem that supports the `security.capability` extended attribute.

```c
int fd = open("/path/to/program", O_RDONLY);
lfcaps_t caps;
int result = lfcaps_readfd(&caps, fd);
close(fd);
```

### `lfcaps_write`

```c
int lfcaps_write(lfcaps_t *caps, const char *path);
```

Writes the permitted and inheritable capability sets in `*caps` to `path` using the Linux `security.capability` extended attribute.

Return values:

- `0`: the capability xattr was written successfully.
- A negative value: the operation failed and the value represents the negated `errno` value.

The `rootid` member is passed through to the underlying Linux file capability representation. The effective flag and the kernel capability format are handled by the implementation.

```c
lfcaps_t caps = {0};
caps.permitted = LFCAP_NET_BIND_SERVICE;

int result = lfcaps_write(&caps, "/path/to/program");
```

Writing capabilities requires sufficient privileges and may fail with errors such as `EPERM`, `EACCES`, or `ENOTSUP`.

### `lfcaps_writefd`

```c
int lfcaps_writefd(lfcaps_t *caps, int fd);
```

Writes the capabilities in `*caps` to an already open file descriptor. It has the same return values and privilege requirements as `lfcaps_write`.

```c
int result = lfcaps_writefd(&caps, fd);
```

The path-based and descriptor-based interfaces are separate deliberately: `lfcaps_read`/`lfcaps_write` take a path, while `lfcaps_readfd`/`lfcaps_writefd` always operate on the supplied descriptor.

### `lfcaps_sprint`

```c
int lfcaps_sprint(char *buf, lfcaps_capset_t capset, char separator);
```

Converts a capability bitset into a separator-delimited string of capability names. The public convenience macro supplies `','` as the default separator when no separator is provided.

For example, a set containing `CAP_CHOWN` and `CAP_NET_RAW` can be formatted as:

```text
chown,net_raw
```

The function writes a terminating null byte and returns the number of characters written, excluding the terminating null byte. The caller must provide a sufficiently large buffer. `LFCAPS_MAXSTRLEN` is provided as a suitable maximum-size constant for normal use.

```c
char buffer[LFCAPS_MAXSTRLEN];
lfcaps_sprint(buffer, caps.permitted, ',');
```

The names and their bit positions are defined centrally in `cap_table.h`.

### `lfcaps_strtocap`

```c
lfcaps_capset_t lfcaps_strtocap(const char *str, char separator);
```

Looks up a capability name and returns the corresponding single-bit capability value. The public convenience macro allows the separator to be omitted; the default separator is `0`.

```c
lfcaps_capset_t cap = lfcaps_strtocap("net_raw");
```

The separator is useful when parsing a token from a delimited list. The function returns `0` when the name is not found. For parsing several names into one set, call it for each token and combine the results with bitwise OR:

```c
lfcaps_capset_t capset = 0;
capset |= lfcaps_strtocap("chown");
capset |= lfcaps_strtocap("net_raw");
```

The standalone tool also provides a fuzzy name parser for its `-n` option, allowing capability names to be matched within comma-separated input.

## Building

cut the sources at the 'standalone' mark to use the layer functions.

## Files

- `lfcaps.h` - public C types, constants, and function declarations.
- `lfcaps.c` - library implementation and standalone command-line tool.
- `cap_table.h` - central capability name and number table.



## License

See [`LICENSE`](LICENSE) for the license terms.
