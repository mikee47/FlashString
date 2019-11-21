String
======

.. highlight:: C++

Introduction
------------

Strings are basically just arrays of char, but have additional methods to allow them to be used more easily. 
These methods are consistent with Wiring Strings, so should be reasonably familiar.

- The ``length()`` method returns the number of characters in the String, excluding the NUL terminator
- The ``size()`` method returns the number of bytes of storage used

For example, "123" is actually stored as ``{ '1', '2', '3', '\0' }`` so the length is 3 and the size is 4.
However, "1234" is stored as ``{ '1', '2', '3', '4', '\0' }`` so the length is 4 and the size is 8.


Using Strings
-------------

.. note::

   You can use ``FSTR::String`` or the Sming-provided ``FlashString`` alias to work with Strings.

Within a function::

   DEFINE_FSTR_LOCAL(myFlashString, "This is my flash string");

   Serial.println(myFlashString);
   Serial.printf("myFlashString has %u chars and occupies %u bytes\n", myFlashString.length(), myFlashString.size());

To use Strings across translation units, we do this in the header::

   DECLARE_FSTR(myFlashString);

And in a source file::

   DEFINE_FSTR(myFlashString, "I am a flash string\0I've got a Naughty NUL.");

You can generally use a Flash String anywhere you can use a regular Wiring String as it has
an implicit *::String()* operator. Note that ``WString`` is used within the library for disambiguation.


Inline Strings
--------------

Use the ``FS()`` macro to create Flash Strings inline::

   Serial.println(FS("A Flash String"));

.. note::

   The macro makes use of ``FS_PTR()`` which creates the structure and returns a pointer to it.
   It therefore behaves like a function call, although the compiler inlines the code.

   Therefore FS() may only be used within functions. At file scope you'll get this error:

      *statement-expressions are not allowed outside functions nor in template-argument lists*

The example above doesn't provide any improvement over an ``F()`` macro as there are no
Flash String overloads available, so is equivalent to this::

   String s = FS("A Flash String");
   Serial.println(s);

However, it's rather different if you pass it to a function which recognises Flash Strings, like this::

   FSTR::println(Serial, FS("A Flash String"));

This is equivalent to::

   FS("A Flash String").printTo(Serial);
   Serial.println();

The printTo() method uses no heap and imposes no restriction on the string length.



Nested Inline Strings
---------------------

It would be really useful to be able to use inline Strings this within nested structures,
and this can be done **provided those structures are in RAM**.

.. important:: Inline Strings cannot be used when defining Vectors or Maps.

Here's is a simplified structure we will attempt to initialize::

      static const struct {
         FlashString* string;
      } flashData PROGMEM = { FS_PTR("Inline Flash String") };
      Serial.println(*flashData.string);

The static *flashData* structure gets initialised at runtime on first use, as per C++ rules.
This attempts to copy our pointer into the `flashData` structure which clearly it cannot do
as it's in PROGMEM, so we get a LOAD/STORE error. We must remove PROGMEM.


Additional Macros
-----------------

LOAD_FSTR(name, fstr)
   Load a String into a named local (stack) buffer.
   Faster than using a temporary Wiring String and avoids using the heap::

      DEFINE_FSTR(globalTest, "This is a testing string");

      void func()
      {
         LOAD_FSTR(local, globalTest);
         printf("%s, %u characters, buffer is %u bytes\n", local, globalTest.length(), sizeof(local));
      }

FSTR_ARRAY(name, str)
   Define a flash String and load it into a named char[] buffer on the stack. This::

      FSTR_ARRAY(buffer, "text");

   Is roughly equivalent to::

      char name[] = "text";

   Except the buffer is word aligned, so *sizeof(name)* may differ.
