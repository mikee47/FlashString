Change Log
==========

Sming 3.7.0
-----------

Here's the PR summary from the original repository. The 'pull-request' links are rendered during Sming documentation build.

Thu Sep 20 18:00:58 2018 :pull-request:`1438`
   Initial commit, the FlashString class plus macros

Fri Oct 5 14:52:24 2018 :pull-request:`1459`
   Add FSTR_TABLE() macro to define a *FlashString\** pointer.
   Very basic table support so we can use it for looking up HTTP status strings, etc.

Wed Oct 24 09:13:50 2018 :pull-request:`1502`
   Add streaming support
   -  Add FlashMemoryStream class
   -  Add FlashTemplateStream class
   -  Add IMPORT_FSTR macro to bind a file into firmware image as FlashString object
   -  Demonstrate usage using HttpServer_ConfigNetwork sample application. Note behaviour is unchanged, but settings.html is statically linked rather than using SPIFFS.

Sat May 11 14:25:05 2019 :pull-request:`1690`
   Mark *const FlashString&* references as PROGMEM. The compiler still stores a pointer to long address
   so it can use relative addressing modes, and it puts these in RAM unless you tell it not to.

   Also added `DEFINE_FSTR_LOCAL` macro to allow correct use inside function definitions.

Thu May 30 14:49:06 2019 :pull-request:`1692`
   Sming Host Emulator!
   The *IMPORT_FSTR* feature requires architecture-dependent compiler support,
   so a separate version was added to support WIN32.
   The standard version works as-is for Linux.

Tue Jun 4 07:43:29 2019 :pull-request:`1709`
    Fix array bounds issue detected via GCC-8 on Host build (#1709)

Mon Jul 8 07:21:58 2019 :pull-request:`1757`
    Bugfix: `LOAD_FSTR()` doesn't NUL-terminate buffered string (#1757)

Tue Jul 30 19:55:50 2019 :pull-request:`1786`
   Revise IMPORT_FSTR macro so the name can itself be #defined (#1786)
    
Mon Oct 21 08:00:48 2019 :pull-request:`1899`
    Add optional `len` parameter to Flashstring::isEqual(const char*)


Sming 4.0.0
-----------

Sat Nov 2 13:01:20 2019

Sming version 4.0.0 is due for release next week, and FlashString has proven to be a useful
addition to the toolkit. Time for an upgrade.

Sming 4.0.1
-----------

FlashString has been expanded from a single file into its own Component library to:

1. Improve support for tables and add associative mapping
2. Ensure each class has its own header file
3. Add methods so users don't need to muck about with memcpy_P, etc.
4. Move some code out of the header into a separate .cpp source file
5. Add proper documentation
6. Make it easier to port to other frameworks (e.g. Arduino)
7. Get it out of Wiring - doesn't really belong there and certainly don't want to clutter up that directory
8. Improve maintainability
9. Fix compatibility with ArduinoJson 6.13.0

Change summary:

-  Move FlashString into a separate Component
-  Add FlashString::read() and readFlash() methods
-  Revise internals to be more consistent with naming
-  Improve table handling using new `Vector` class
-  Add associative mapping support with `Map` class
-  Revise structures so it contains only the length, obtaining data position using pointer arithmetic.
   This fixes an error with GCC 8.2.0 which didn't like accessing zero-length arrays.
-  Use ObjectBase as the first element in a data structure to allow use of 'safe' static casting,
   handled using the `as()` method template
-  Documentation!
