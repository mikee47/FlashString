Change Log
==========

I'm going to break with convention here any lay this out in date order.

Sming 3.7.0
-----------

20/09/2018 :pull-request:`1438` Add *FlashString.h* to Wiring directory
   The FlashString class plus macros

05/10/2018 :pull-request:`1459` Add FSTR_TABLE() macro to define a *FlashString\** pointer
   Very basic table support so we can use it for looking up HTTP status strings, etc.

24/10/2018 :pull-request:`1502` Add streaming support
   

11/05/2019 :pull-request:`1690` Fix *const FlashString&* references consuming RAM


30/05/2019 :pull-request:`1692` Sming Host Emulator
   The *IMPORT_FSTR* feature requires architecture-dependent compiler support, so a separate
   version was added to support WIN32. The standard version works as-is for Linux.



Version history
---------------

Why an upgrade?
---------------

FlashString was first introduced in Sming 3.7.0 on 17 November 2018, as a single file in the Wiring directory.
It hasn't seen any development since, other than incorporating support for the Host Emulator in Sming 4.0.0.

It's November 2019 and following compatibility issues with ArduinoJson version 6.13.0 it seems high time
for a review and upgrade.

FlashString has been separated out into its own Component library to:

1. Improve support for tables and add associative mapping
2. Ensure each class has its own header file
3. Add methods so users don't need to muck about with memcpy_P, etc.
4. Move some code out of the header into a separate .cpp source file
5. Add proper documentation
6. Make it easier to port to other frameworks (e.g. Arduino)
7. Get it out of Wiring - doesn't really belong there and certainly don't want to clutter up that directory
8. Improve maintainability

Changes from Sming 4.0.0
------------------------


