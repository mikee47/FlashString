Change Log
==========

Sming 3.7.0
-----------

FlashString was first introduced in Sming 3.7.0 on 17 November 2018, as a single file in the Wiring directory.
Other than bugfixes and Host Emulator support it hasn't had any significant updates.

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

FlashString has been expanded from a single file into its own Component library to:

1. Improve support for tables and add associative mapping
2. Ensure each class has its own header file
3. Add methods so users don't need to muck about with memcpy_P, etc.
4. Move some code out of the header into a separate .cpp source file
5. Add proper documentation
6. Make it easier to port to other frameworks (e.g. Arduino)
7. Get it out of Wiring - doesn't really belong there and certainly don't want to clutter up that directory
8. Improve maintainability

Change summary:

-  Add FlashString::read() and readFlash() methods
-  Revise FlashString internals to be more consistent with naming
-  Improve FlashString table handling using new `FlashStringTable` class
-  Add `FlashStringMap` and `FlashStringIntMap` classes
-  Investigate alternative structures to allow copy construction. Nope, not going to happen.
   Cannot use constructors at all as with global instances produces constructor code which attempts
   to write to flashmem. These MUST be vanilla structs to work correctly.
-  Revise FlashString structure so it contains only the length,
   obtaining data position using pointer arithmetic.
   This fixes an error with GCC 8.2.0 which didn't like accessing zero-length arrays.
-  All data structures use the appropriate class as the first element
   -  Avoids violating any compiler rules
   -  No need for casting
   -  Clean way to get at the actual FlashString
-  Move FlashString into Component
-  Documentation!

Upgrade notes
~~~~~~~~~~~~~

.. highlight:: c++

FlashString tables have been improved. Previously::

   DEFINE_FSTR_LOCAL(fstr1, "Test string #1");
   DEFINE_FSTR_LOCAL(fstr2, "Test string #2");
   
   static FSTR_TABLE(table) = {
      FSTR_PTR(fstr1),
      FSTR_PTR(fstr2),
   };

   Serial.println("FSTR tables[%u]\n", ARRAY_SIZE(table));
   Serial.printf(" fstr1 = '%s'\n", String(*table[0]).c_str());
   Serial.printf(" fstr1.length() = %u\n", table[0]->length());

Now you can do this::

   DEFINE_FSTR_DATA_LOCAL(data1, "Test string #1");
   DEFINE_FSTR_DATA_LOCAL(data2, "Test string #2");

   DEFINE_FSTR_TABLE(table, &data1.fstr, &data2.fstr);

   Serial.printf("FSTR table[%u]\n", table.length());
   Serial.printf(" fstr1 = '%s'\n", String(table[0]).c_str());
   Serial.printf(" fstr1.length() = %u\n", table[0].length());

Two mapping classes have also been added to support keys using a FlashString or an integral type.

