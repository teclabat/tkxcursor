# XCursor Extension for Tcl/Tk

This extension adds support for X Cursor library cursor files in Tk applications.

## What It Does

Provides the `xcursor` command for loading cursor files in Xcursor format and applying them to Tk windows. This enables using system cursor themes and custom cursor designs in your Tk applications.

## Building

Standard TEA build process:

```bash
./configure --with-tcl=/path/to/tclConfig.sh --with-tk=/path/to/tkConfig.sh
make
make install
```

### Build Requirements

- Tcl 8.6+
- Tk 8.6+
- X11 headers
- libXcursor development files
- autoconf, gcc/g++

## Using in Your Application

```tcl
package require XCursor 1.0

# Apply cursor to a window
xcursor .mybutton /usr/share/icons/Adwaita/cursors/hand1

# Clear cursor (restore default)
xcursor .mywindow ""
```

### Command Format

`xcursor` *window* *filepath*

Where:
- *window* is any Tk window path
- *filepath* is the path to an Xcursor file, or empty string to clear

### Error Conditions

Returns an error if:
- Wrong number of arguments provided
- Window does not exist
- File cannot be loaded or is invalid
- Memory allocation fails

## Implementation Details

Cursors are cached after loading. The same cursor file loaded multiple times reuses the cached cursor object. All resources are freed when the interpreter exits.

## Platform Requirements

X11 only. Not compatible with Windows or macOS native windowing.

## License

GNU General Public License v2 or later. See LICENSE file.
