Flash Strings
=============

.. highlight:: C++

Introduction
------------

Strings are basically just arrays of char, but have additional methods to allow them to be used more easily. 
These methods are consistent with Wiring :cpp:class:`String`, so should be reasonably familiar.

- :cpp:func:`length() <FSTR::Object::length>` returns the number of characters in the String, excluding the NUL terminator
- :cpp:func:`size() <FSTR::String::size>` returns the number of bytes of storage used

For example, "123" is actually stored as ``{ '1', '2', '3', '\0' }`` so the length is 3 and the size is 4.
However, "1234" is stored as ``{ '1', '2', '3', '4', '\0' }`` so the length is 4 and the size is 8.


Using Strings
-------------

.. note::

   You can use ``FSTR::String`` or the Sming-provided ``FlashString`` alias to work with Strings.

Within a function::

   DEFINE_FSTR_LOCAL(myFlashString, "This is my flash string");

   Serial.println(myFlashString);
   Serial << "myFlashString has " << myFlashString.length() << " chars and occupies " << myFlashString.size() << " bytes" << endl;

To use Strings across translation units, we do this in the header::

   DECLARE_FSTR(myFlashString);

And in a source file::

   DEFINE_FSTR(myFlashString, "I am a flash string\0I've got a Naughty NUL.");

You can generally use a Flash String anywhere you can use a regular Wiring String as it has
an implicit *::String()* operator. Note that ``WString`` is used within the library for disambiguation.

See also :cpp:class:`CStringArray`.


Inline Strings
--------------

Use the :c:func:`FS` macro to create Flash Strings inline::

   Serial.println(FS("A Flash String"));

.. note::

   The macro makes use of ``FS_PTR()`` which creates the structure and returns a pointer to it.
   It behaves like a function call, although the compiler inlines the code.

   Therefore FS() may only be used within functions. At file scope you'll get this error:

      *statement-expressions are not allowed outside functions nor in template-argument lists*

The example above doesn't provide any improvement over :c:macro:`F` as there are no
Flash String overloads available, so is equivalent to this::

   String s = FS("A Flash String");
   Serial.println(s);

However, it's rather different if you pass it to a function which recognises Flash Strings, like this::

   FSTR::println(Serial, FS("A Flash String"));

This is equivalent to::

   FS("A Flash String").printTo(Serial);
   Serial.println();

:cpp:func:`FSTR::String::printTo` uses no heap and imposes no restriction on the string length.

.. note::

   Sming contains general streaming support via :cpp:class:`Print`, so FlashString objects can
   be printed like this::

      Serial.println(myFlashString);
      Serial << myFlashString << endl;

   This is implemented via non-virtual `printTo` overrides.


Nested Inline Strings
---------------------

It would be really useful to be able to use inline Strings within nested structures,
and this can be done **provided those structures are in RAM**.

.. important:: Inline Strings cannot be used when defining Vectors or Maps.

Here's is a simplified structure we will attempt to initialize::

      static const struct {
         const FlashString* string;
      } flashData PROGMEM = { FS_PTR("Inline Flash String") };
      Serial.println(*flashData.string);

The static *flashData* structure gets initialised at runtime on first use, as per C++ rules.
This attempts to copy our pointer into the `flashData` structure which clearly it cannot do
as it's in :c:macro:`PROGMEM`, so we get a LOAD/STORE error. We must remove PROGMEM.


Avoiding the heap
-----------------

Instead of using a temporary Wiring String, you can use :c:func:`LOAD_FSTR` to load the
content into a temporary stack buffer::

      DEFINE_FSTR(globalTest, "This is a testing string");

      void func()
      {
         LOAD_FSTR(local, globalTest);
         printf("%s, %u characters, buffer is %u bytes\n", local, globalTest.length(), sizeof(local));
      }

You can do this with inline Flash Strings using :c:func:`FSTR_ARRAY`::

      FSTR_ARRAY(buffer, "text");

Is roughly equivalent to::

   char name[] = "text";

Except the buffer is word aligned, so *sizeof(name)* may differ.


Macros
------

.. doxygengroup:: fstr_string
   :content-only:

String Class
------------

.. doxygenclass:: FSTR::String
   :members:
