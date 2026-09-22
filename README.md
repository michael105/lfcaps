# lfcaps

`lfcaps` is a small C library and command-line tool for reading, writing, inspecting, and modifying Linux file capabilities. It provides a user-level representation of permitted and inheritable capability sets and translates them to and from the Linux extended attribute format used by the kernel.

The project is intended for Linux programs and shell workflows that need to inspect or manage file capabilities without dealing directly with the kernel capability xattr layout.

```
this readme is mostly written by ai, everything else with vi (manually).
This aims to have a more user friendly handling of
the capability bitsets than the kernel/linux fcap library.
The capabilities of the fcaps lib/kernel definitions 
are spread into a 'striped' 32bit array,
what might had been neccessary for backwards compatbility.

The combination of commandline flags with the lfcaps tool 
seems to me also a little bit more stringent.

Was the main reason to rewrite the capability get/set functions et al.

```

The static binary is compiled for linux 64bit.
```
bin/lfcaps 6kB
sha3:     d2d36311488e12f159be71512ffa5812642433f3a7341511a6c0197107491bf9
skein256: 9ab31cbc0ffc8181b6a2c9865b47b80a87e4a3c7150a53bb469c6b5abbf3ece5
```


## Usage

- combine the capsets permitted and inheritable into single 64bit bitfields.
- Read file capabilities by path or from an open file descriptor.
- Write permitted and inheritable capability sets by path or file descriptor.
- Support the Linux file capability formats reported as versions 1, 2, and 3.
- Convert capability bitsets to readable names such as `chown` and `net_bind_service`.
- Convert capability names back to a capability bitset.

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

# capabilities can be given as non ambigous substrings
lfcaps -s -n net_bind,net_adm /path/to/program

# Add a capability to the inheritable set,
lfcaps -ai -n net_raw,chroot /path/to/program

# Test whether a file contains a capability
lfcaps -tn net_bind /path/to/program
```

Changing file capabilities requires appropriate privileges, 
`CAP_SETFCAP` or root.


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

The constants are bits of a 64bit bitfield,
documentation should be in man 7 capabilities.


```
set capabilities:

lfcaps_t fc = {0};
fc.permitted = LFCAP_CHROOT | LFCAP_ADMIN;

delete:
fc.permitted &= ~LFCAP_ADMIN;

..
```


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


```c
lfcaps_capset_t cap = lfcaps_strtocap_substr( _str, separator = 0, ambivalence = 0 ) 
```

search for the substring str within the captable names.
set ambivalence to 1, to allow ambivalent substrings,
the first occurance is returned as match.

returns 0, if not found or ambigous (substring is multiple within
the list of capnames, and ambivalence is 0 (default))

```c
//add sys_admin (not ptrace)
lfcaps_capset_t caps = lfcaps_strtocap_substr( "chroot,ptrace" );

// fails (admin is ambigous, there is mac_admin,net_admin,sys_admin) 
caps |= lfcaps_strtocap_substr( "admin,ptr", ',' ); 

// ok, add mac_admin
caps |= lfcaps_strtocap_substr( "admin,ptr", ',', 1 ); 
```


## Building

include lfcaps.h, lfcaps.c and cap_table.h to use the layer functions


## Files

- `lfcaps.h` - public C types, constants, and function declarations.
- `lfcaps.c` - library implementation and standalone command-line tool.
- `cap_table.h` - central capability name and number table.



## License

BSD 2clause.

See [`LICENSE`](LICENSE) for the license terms.

