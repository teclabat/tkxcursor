# X Cursor Extension for Tcl/Tk

A Tcl/Tk extension that adds support for X Cursor library cursor files in Tk applications.

**Version:** 1.0.0 \
**Package:** `xcursor` \
**Command:** `xcursor`

---

## Table of Contents

1. [Overview](#overview)
2. [Key Features](#key-features)
3. [Supported Platforms](#supported-platforms)
4. [Installation](#installation)
5. [Command Reference](#command-reference)
6. [Usage Examples](#usage-examples)
7. [Cursor File Locations](#cursor-file-locations)
8. [Error Handling](#error-handling)
9. [Best Practices](#best-practices)
10. [Troubleshooting](#troubleshooting)
11. [Technical Details](#technical-details)
12. [License](#license)

---

## Overview

The **xcursor** package provides a Tcl/Tk interface to the X Cursor library (libXcursor), enabling Tk applications running on X11 systems to use modern cursor themes and custom cursor designs from Xcursor-format files.

Traditional Tk applications are limited to the built-in cursor names (`arrow`, `hand2`, `watch`, etc.). This extension breaks that limitation by allowing direct loading of cursor files from system icon themes or custom sources.

## Key Features

- **Load cursor files** in Xcursor format
- **Apply cursors** to any Tk window or widget
- **Use system cursor themes** instead of built-in cursors only
- **Automatic cursor caching** for efficient reuse
- **Simple API** with a single command
- **Resource cleanup** on interpreter exit
- **Clear cursors** to restore defaults

## Supported Platforms

**X11 Only:**
- Linux
- BSD (FreeBSD, OpenBSD, NetBSD)
- Unix systems with X11

**Not Compatible:**
- Windows (native or WSL)
- macOS (Aqua windowing)

---

## Installation

```tcl
package require xcursor
```

### Requirements

**Build-time**:
- Tcl 8.6 or later
- Tk 8.6 or later
- C compiler (gcc/g++)
- X11 development headers
- libXcursor development files (`libxcursor-dev` or `libXcursor-devel`)
- autoconf and GNU Make

**Runtime**:
- Tcl 8.6+
- Tk 8.6+
- X11 display server
- libXcursor library

### Building from Source

Standard TEA (Tcl Extension Architecture) build process:

```bash
cd D:\CM.tcltk\tkxcursor
./configure --with-tcl=/path/to/tclConfig.sh --with-tk=/path/to/tkConfig.sh
make
make test
make install
```

### Installation on Debian/Ubuntu

```bash
# Install build dependencies
sudo apt-get install tcl-dev tk-dev libxcursor-dev build-essential

# Build and install
./configure
make
sudo make install
```

### Installation on Fedora/RedHat

```bash
# Install build dependencies
sudo dnf install tcl-devel tk-devel libXcursor-devel gcc make

# Build and install
./configure
make
sudo make install
```

---

## Command Reference

### `xcursor`

Applies an Xcursor file to a Tk window or widget, or clears the cursor.

**Syntax:**
```tcl
xcursor window filepath
```

**Parameters:**

- `window` - Tk window path (e.g., `.`, `.mybutton`, `.frame.label`)
- `filepath` - Absolute or relative path to Xcursor file, or empty string to clear

**Returns:**

- Empty string on success

**Errors:**

Returns a Tcl error with one of these messages:
- `"Invalid command count, use: xcursor <tkwin> <cursorfile>"` - Wrong number of arguments
- `"bad window path name \"...\"..."` - Window does not exist
- `"invalid cursor file '...'"` - File cannot be loaded or is not a valid Xcursor file

**Example:**
```tcl
# Apply cursor from system theme
xcursor .mybutton /usr/share/icons/Adwaita/cursors/hand1

# Clear cursor (restore default)
xcursor .mybutton ""
```

---

## Usage Examples

### Example 1: Basic Cursor Application

```tcl
package require Tk
package require xcursor

# Create a button
button .btn -text "Hover over me" -command exit

# Apply a hand cursor from system theme
xcursor .btn /usr/share/icons/Adwaita/cursors/hand2

pack .btn -padx 20 -pady 20
```

---

### Example 2: Different Cursors on Different Widgets

```tcl
package require Tk
package require xcursor

# Create multiple widgets
label .lbl -text "Read-only text"
entry .ent
button .btn -text "Click me"

# Apply different cursors
xcursor .lbl /usr/share/icons/Adwaita/cursors/text
xcursor .ent /usr/share/icons/Adwaita/cursors/xterm
xcursor .btn /usr/share/icons/Adwaita/cursors/hand2

pack .lbl .ent .btn -padx 10 -pady 5
```

---

### Example 3: Clearing Cursors

```tcl
package require Tk
package require xcursor

button .btn -text "Toggle cursor"

set cursor_active 0

proc toggle_cursor {} {
    global cursor_active

    if {$cursor_active} {
        # Clear custom cursor
        xcursor .btn ""
        set cursor_active 0
        .btn configure -text "Custom cursor OFF"
    } else {
        # Apply custom cursor
        xcursor .btn /usr/share/icons/Adwaita/cursors/watch
        set cursor_active 1
        .btn configure -text "Custom cursor ON"
    }
}

.btn configure -command toggle_cursor
pack .btn -padx 20 -pady 20
```

---

### Example 4: Error Handling

```tcl
package require Tk
package require xcursor

proc apply_cursor_safe {window cursor_file} {
    if {[catch {xcursor $window $cursor_file} error]} {
        puts stderr "Error applying cursor: $error"
        return 0
    }
    return 1
}

button .btn -text "Test Button"
pack .btn

# Try to apply cursor with error handling
if {[apply_cursor_safe .btn /usr/share/icons/Adwaita/cursors/hand2]} {
    puts "Cursor applied successfully"
} else {
    puts "Failed to apply cursor, using default"
}
```

---

### Example 5: Custom Cursor Theme

```tcl
package require Tk
package require xcursor

# Set base path to custom cursor theme
set cursor_theme_path "/usr/share/icons/MyCustomTheme/cursors"

# Create application with themed cursors
frame .toolbar -relief raised -borderwidth 2
button .toolbar.new -text "New" -width 8
button .toolbar.open -text "Open" -width 8
button .toolbar.save -text "Save" -width 8
pack .toolbar.new .toolbar.open .toolbar.save -side left -padx 2 -pady 2

text .editor -width 60 -height 20
pack .toolbar -side top -fill x
pack .editor -side top -fill both -expand yes

# Apply cursors
xcursor .toolbar.new "$cursor_theme_path/hand2"
xcursor .toolbar.open "$cursor_theme_path/hand2"
xcursor .toolbar.save "$cursor_theme_path/hand2"
xcursor .editor "$cursor_theme_path/xterm"
```

---

### Example 6: Dynamic Cursor Changes

```tcl
package require Tk
package require xcursor

canvas .c -width 400 -height 300 -bg white
pack .c

set cursor_path "/usr/share/icons/Adwaita/cursors"

# Change cursor based on canvas state
proc update_cursor {mode} {
    global cursor_path

    switch $mode {
        draw    { xcursor .c "$cursor_path/pencil" }
        select  { xcursor .c "$cursor_path/hand1" }
        move    { xcursor .c "$cursor_path/fleur" }
        wait    { xcursor .c "$cursor_path/watch" }
        default { xcursor .c "" }
    }
}

# Create toolbar
frame .tools
radiobutton .tools.draw -text "Draw" -variable mode -value draw \
    -command {update_cursor draw}
radiobutton .tools.select -text "Select" -variable mode -value select \
    -command {update_cursor select}
radiobutton .tools.move -text "Move" -variable mode -value move \
    -command {update_cursor move}
pack .tools.draw .tools.select .tools.move -side left
pack .tools -side top

# Initialize with draw mode
set mode draw
update_cursor draw
```

---

### Example 7: Progress Indicator

```tcl
package require Tk
package require xcursor

button .btn -text "Start Long Operation" -command start_operation
label .status -text "Ready"
pack .btn .status -padx 20 -pady 10

proc start_operation {} {
    # Show wait cursor
    xcursor . /usr/share/icons/Adwaita/cursors/watch
    .status configure -text "Working..."
    update

    # Simulate long operation
    after 3000 finish_operation
}

proc finish_operation {} {
    # Restore default cursor
    xcursor . ""
    .status configure -text "Complete!"
}
```

---

### Example 8: Context-Sensitive Cursors

```tcl
package require Tk
package require xcursor

canvas .c -width 500 -height 400 -bg white
pack .c -fill both -expand yes

set cursor_base "/usr/share/icons/Adwaita/cursors"

# Create zones
.c create rectangle 50 50 200 200 -fill lightblue -tags zone1
.c create rectangle 250 50 450 200 -fill lightgreen -tags zone2
.c create rectangle 50 250 200 350 -fill lightyellow -tags zone3
.c create rectangle 250 250 450 350 -fill lightcoral -tags zone4

.c create text 125 125 -text "Clickable\nArea" -tags zone1
.c create text 350 125 -text "Draggable\nArea" -tags zone2
.c create text 125 300 -text "Editable\nArea" -tags zone3
.c create text 350 300 -text "Info\nArea" -tags zone4

# Bind cursor changes to zones
.c bind zone1 <Enter> {xcursor .c "$cursor_base/hand2"}
.c bind zone2 <Enter> {xcursor .c "$cursor_base/fleur"}
.c bind zone3 <Enter> {xcursor .c "$cursor_base/xterm"}
.c bind zone4 <Enter> {xcursor .c "$cursor_base/question_arrow"}

# Restore default when leaving zones
foreach zone {zone1 zone2 zone3 zone4} {
    .c bind $zone <Leave> {xcursor .c ""}
}
```

---

## Cursor File Locations

### System Cursor Themes

Common locations for system cursor themes on Linux:

**Adwaita Theme (GNOME default):**
```
/usr/share/icons/Adwaita/cursors/
```

**DMZ Theme:**
```
/usr/share/icons/DMZ-White/cursors/
/usr/share/icons/DMZ-Black/cursors/
```

**Oxygen Theme (KDE):**
```
/usr/share/icons/oxygen/cursors/
```

**User-Installed Themes:**
```
~/.icons/[theme-name]/cursors/
~/.local/share/icons/[theme-name]/cursors/
```

### Common Cursor Names

Standard cursor file names found in most themes:

| Cursor Name | Purpose |
|-------------|---------|
| `default` | Default pointer |
| `pointer` | Pointing hand |
| `hand1`, `hand2` | Hand cursors |
| `left_ptr` | Left-pointing arrow |
| `right_ptr` | Right-pointing arrow |
| `xterm` | Text selection I-beam |
| `text` | Text cursor |
| `watch` | Busy/wait indicator |
| `wait` | Wait cursor |
| `crosshair` | Crosshair for precise selection |
| `fleur` | Move/drag (four arrows) |
| `question_arrow` | Help cursor |
| `pencil` | Drawing cursor |
| `circle` | Circle/target cursor |
| `plus` | Precision crosshair |
| `sb_h_double_arrow` | Horizontal resize |
| `sb_v_double_arrow` | Vertical resize |
| `top_left_corner` | Resize top-left |
| `top_right_corner` | Resize top-right |
| `bottom_left_corner` | Resize bottom-left |
| `bottom_right_corner` | Resize bottom-right |

### Finding Installed Cursor Themes

```bash
# List all cursor themes
find /usr/share/icons -type d -name cursors

# List cursor files in a specific theme
ls /usr/share/icons/Adwaita/cursors/

# List user-installed themes
ls ~/.icons/*/cursors/ 2>/dev/null
ls ~/.local/share/icons/*/cursors/ 2>/dev/null
```

---

## Error Handling

### Common Errors and Solutions

#### Wrong Number of Arguments

**Error:**
```
Invalid command count, use: xcursor <tkwin> <cursorfile>
```

**Cause:** Command called with incorrect number of arguments

**Solution:**
```tcl
# Wrong - missing argument
xcursor .btn

# Correct
xcursor .btn /path/to/cursor
```

---

#### Invalid Window Path

**Error:**
```
bad window path name ".nonexistent"
```

**Cause:** Specified window does not exist

**Solution:**
```tcl
# Create window before applying cursor
button .btn -text "Click"
pack .btn
xcursor .btn /usr/share/icons/Adwaita/cursors/hand2
```

---

#### Invalid Cursor File

**Error:**
```
invalid cursor file '/path/to/cursor'
```

**Causes:**
- File does not exist
- File is not in Xcursor format
- Insufficient permissions to read file
- Corrupted cursor file

**Solutions:**
```bash
# Check file exists
ls -l /path/to/cursor

# Check file permissions
chmod 644 /path/to/cursor

# Verify it's an Xcursor file
file /usr/share/icons/Adwaita/cursors/hand2
# Should show: X11 cursor
```

---

## Best Practices

### 1. Check Window Existence

Always ensure the window exists before applying a cursor:

```tcl
# Good practice
if {[winfo exists .mywidget]} {
    xcursor .mywidget /path/to/cursor
}

# Better practice with error handling
if {[catch {xcursor .mywidget /path/to/cursor} error]} {
    puts stderr "Failed to set cursor: $error"
}
```

### 2. Use Absolute Paths or Validate Files

```tcl
# Good - absolute path
xcursor .btn /usr/share/icons/Adwaita/cursors/hand2

# Better - verify file exists first
set cursor_file /usr/share/icons/Adwaita/cursors/hand2
if {[file exists $cursor_file] && [file readable $cursor_file]} {
    xcursor .btn $cursor_file
} else {
    puts stderr "Cursor file not found: $cursor_file"
}
```

### 3. Clear Cursors When Done

Restore default cursors when they're no longer needed:

```tcl
# Set custom cursor during operation
xcursor . /usr/share/icons/Adwaita/cursors/watch
# ... long operation ...
# Clear when done
xcursor . ""
```

### 4. Cache Paths for Reuse

The extension automatically caches loaded cursors, but you should still organize your cursor paths:

```tcl
# Define cursor paths once
namespace eval cursors {
    variable base /usr/share/icons/Adwaita/cursors
    variable hand "$base/hand2"
    variable wait "$base/watch"
    variable text "$base/xterm"
    variable move "$base/fleur"
}

# Use throughout application
xcursor .btn $cursors::hand
xcursor .text $cursors::text
```

### 5. Handle Missing Themes Gracefully

```tcl
proc apply_cursor_with_fallback {window cursor_file {fallback ""}} {
    if {[file exists $cursor_file]} {
        if {[catch {xcursor $window $cursor_file}]} {
            # Failed to apply, use fallback or default
            xcursor $window $fallback
        }
    } else {
        # File doesn't exist, use fallback
        xcursor $window $fallback
    }
}

# Usage
apply_cursor_with_fallback .btn \
    /usr/share/icons/Adwaita/cursors/hand2 \
    ""  ;# fallback to default
```

### 6. Don't Overuse Custom Cursors

Use custom cursors purposefully:

```tcl
# Good - meaningful cursor changes
xcursor .canvas.text_area /usr/share/icons/Adwaita/cursors/xterm
xcursor .canvas.draw_area /usr/share/icons/Adwaita/cursors/pencil

# Bad - unnecessary cursor changes
xcursor . /usr/share/icons/Adwaita/cursors/pirate  ;# confusing
```

---

## Troubleshooting

### Cursor Doesn't Change

**Symptoms:** Cursor remains the same after `xcursor` command

**Possible Causes:**
1. Running on non-X11 platform (Windows, macOS)
2. File path is incorrect
3. File is not a valid Xcursor file
4. X11 display issue

**Debugging Steps:**

```tcl
# 1. Verify X11 platform
puts [tk windowingsystem]  ;# Should be "x11"

# 2. Check file exists
set cursor /usr/share/icons/Adwaita/cursors/hand2
puts [file exists $cursor]  ;# Should be 1

# 3. Try with error catching
if {[catch {xcursor .btn $cursor} error]} {
    puts "Error: $error"
}

# 4. Verify window exists
puts [winfo exists .btn]  ;# Should be 1
```

---

### Extension Won't Load

**Symptoms:** `package require xcursor` fails

**Solutions:**

```bash
# Check if package is installed
find /usr/lib /usr/local/lib -name "*xcursor*"

# Check package path
tclsh
% puts $auto_path
% package require xcursor
```

**Fix:** Reinstall the package or add installation directory to `auto_path`:

```tcl
lappend auto_path /path/to/xcursor/installation
package require xcursor
```

---

### Cursor Files Not Found

**Symptoms:** `invalid cursor file` error for standard paths

**Cause:** Cursor theme not installed

**Solution:** Install cursor themes:

```bash
# Debian/Ubuntu
sudo apt-get install adwaita-icon-theme dmz-cursor-theme

# Fedora/RedHat
sudo dnf install adwaita-icon-theme dmz-cursor-theme

# Find what's installed
ls /usr/share/icons/*/cursors/
```

---

### Memory Issues (Advanced)

**Symptoms:** Application memory grows over time

**Cause:** Known bug - cursors are cached but not properly freed on cleanup

**Workaround:**
- Limit number of unique cursor files loaded
- Restart application periodically if loading many cursors
- Use built-in Tk cursors where possible

**Note:** This is a known issue in the current implementation. See [Technical Details](#technical-details) for more information.

---

## Technical Details

### Cursor Caching

The extension maintains an internal cache of loaded cursors:

- **First load:** Cursor file is loaded from disk and cached
- **Subsequent loads:** Cached cursor is reused
- **Cache key:** Full file path (string comparison)
- **Lifetime:** Until interpreter exits

**Performance Implications:**
- Loading same cursor file multiple times is efficient (O(n) lookup where n = cached cursors)
- Each unique file path creates a new cache entry
- Symbolic links to same cursor file create separate cache entries

### Memory Management

**Current Implementation:**
- Cursors cached in a linked list
- Memory allocated for each unique cursor file path
- **Known Issue:** Cleanup function doesn't properly free X11 cursor resources due to NULL display pointer

**Resource Usage:**
- Minimal memory per cached cursor (~50 bytes + path length)
- X11 server resources: Each cached cursor consumes server-side memory

### Thread Safety

**Current Status:** Not thread-safe

- Uses global static cache variable
- No mutex protection
- Not safe for multi-threaded Tcl applications

**Recommendation:** Use only from main thread

### Multi-Interpreter Support

**Current Status:** Not safe for multiple interpreters

- Cache is global across all interpreters in same process
- Cleanup in one interpreter affects others
- May cause crashes if multiple interpreters are created/destroyed

**Recommendation:** Use in single-interpreter applications only

### X11 Display Dependencies

The extension requires:
- Active X11 display connection
- Tk initialized (`package require Tk` before `package require xcursor`)
- Valid Tk main window

**Will not work with:**
- Headless/display-less environments
- Windows or macOS native windowing
- Virtual displays without X11 extension support

---

## See Also

- **Xcursor Library:** libXcursor documentation
- **X11 Cursor Specification:** X11R7 cursor documentation
- **Tk Cursor Option:** `man 3tk options` (cursor option)
- **Icon Themes:** [freedesktop.org Icon Theme Specification](https://specifications.freedesktop.org/icon-theme-spec/)

### Related Tk Commands

```tcl
# Built-in Tk cursor option (string names only)
button .btn -cursor hand2

# Get current cursor
.btn cget -cursor

# Available with xcursor extension
xcursor .btn /usr/share/icons/Adwaita/cursors/hand2
```

---

## License

GNU General Public License v2 or later (GPL-2.0+)

Copyright (c) 2012 Thomas Perschak \
Copyright (c) 2025 TERMA Technologies GmbH

See the LICENSE file for full license text.

---

## Version History

**1.0.0** (2025)
- Initial release
- Single `xcursor` command
- Automatic cursor caching
- X11/libXcursor support
- TEA-compliant build system

---

## Contributing

Bug reports and patches welcome. Known issues include:

1. Memory leak in cleanup function
2. Multi-interpreter cache corruption
3. Thread safety issues
4. Linear search performance for large cache

For bug reports, please include:
- Tcl/Tk version (`info patchlevel`)
- X11 server version
- Distribution and version
- Minimal reproduction script

---

*This extension is part of the CM.tcltk project.*
