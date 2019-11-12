FlashString Technobabble
========================

Why not just use a wrapper class?
---------------------------------

A wrapper class would require this::

      const struct {
         ...
      } myFlashData PROGMEM = { ... };

      void func()
      {
         FlashString fs(myFlashData);
         Serial.println(fs);
      }

Easy enough, just create a wrapper every time you need to use the data.

It does get a bit clunky though when you have a lot of strings, lists of strings
or where strings are used in several places.

How about global/static wrappers
--------------------------------

Like this::

   FlashString fs(myFlashData);

   void func()
   {
      Serial.println(fs);
   }

Each wrapper uses 4 bytes of RAM. We also need to bear in mind that at startup
objects are being initialised so we cannot necessarily guarantee that our FlashString
wrappers have been created by the time they're needed in other code. Result: hang/crash.

How about a macro to create the wrapper?
----------------------------------------

Something like this::

   void func()
   {
      Serial.println(FS(myFlashData));
   }

To be honest, I hadn't really considered that one, but it is worth exploring.

Can't I put the wrapper in PROGMEM?
-----------------------------------

A wrapper requires a constructor, each of which needs an initialiser which gets
called at startup.

Which is why FlashString isn't a wrapper.

FlashStrings can be used anywhere you can use PROGMEM data, including globally initialised
classes.

There is a disadvantage to this approach: You can't make copies of FlashString objects.
They must always be accessed as references or pointers.


Reasons to use FlashString
~~~~~~~~~~~~~~~~~~~~~~~~~~

-  Easy to use, treat like read-only ``String`` object
-  Use methods of FlashString instead of calls to memcpy_P, strcpy_P, etc.
-  Blocks of data stored in flash can be passed around easily as the length is known
-  Can be passed to functions instead of a String parameter, but without having to load it into RAM first
-  Can be streamed directly using *FlashMemoryStream*
-  Can be read randomly using *FlashString::read()*
-  Aligned read and copy operations provide excellent performance
-  Fast equality comparisons using length field to short-circuit comparison
-  Data can be imported and linked directly into the program image from a local file,
   and accessed as a FlashString
-  Custom structures can be defined and accessed as a FlashString
-  Operates transparently with ArduinoJson (patched as from version 6.13.0)

Reasons not to use FlashString
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

-  Storing lots of small strings can be inefficient because of length and alignment,
   and because a NUL terminator is always appended by DEFINE_FSTR macros even though it's
   not generally required. (This is because of how C requires string arrays to be declared.)
   
   For example, "A" requires 8 bytes::

.. code-block:: text

      01 00 00 00 // Length
      41 00 00 00 // "A\0" padded to word boundary

   However, this disadvantage can be overcome by storing such strings in a single block
   and accessing them using a :source:`Sming/Core/Data/CStringArray`.

.. note::

   Some counted string implementations store the length field at offset -4 so we
   always point to the actual data. In this case, that doesn't offer any advantages
   as directly accessing the data is discouraged. Therefore, the length field always
   comes first.
