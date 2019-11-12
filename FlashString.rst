FlashString
===========

.. highlight:: C++

There is no difference between a C++ class and a struct, other than the default scope,
but as we're initialising member data as a C structure we use a *struct*.

Without the methods, this is a FlashString::

   struct FlashString {
      uint32_t flashLength;
      // uint8_t data[];
   };

.. note::

   Even though *flashLength* is public, you should avoid accessing it directly
   and use the provided length() method instead.

To define the actual data, you can do this::

   #include <FlashString.h>

   const struct {
      FlashString fstr;
      char data[8];
   } flashHelloData PROGMEM = { {6}, "hello" };

Remember you'll need to add *static* if defined inside a function.

.. attention::

   Hang on! You've allocated 8 chars but "hello" is only 5!

   There are two reasons for this:
   
   1. We need _at least_ 6 characters to store "hello\\0" or the compiler throws it out.
   2. By keeping our sizes aligned to 32-bit word sizes the compiler can produce the most efficient code.

You can then do stuff like::

   Serial.println(flashHelloData.fstr);

Let's use a reference to make life a little easier:: 

   const FlashString& hello = myFlashData.fstr;
   Serial.println(hello);
   Serial.printf("Hello has %u chars and occupies %u bytes\n", hello.length(), hello.size());


Defining FlashStrings
---------------------

OK, so you now know how FlashStrings work but unless you've got some custom data structure to
store you can just use some macros to do all the hard work.

To use FlashStrings across translation units, we do this in the header::

   DECLARE_FSTR(myFlashString);

And in a source file::

   DEFINE_FSTR(myFlashString, "I am a flash string\0I've got a Naughty NUL.");

If FlashStrings are only needed withing a single translation unit, do this::

   DEFINE_FSTR_LOCAL(myFlashString, "This is my secret flash string");

Both versions create a static data structure and a FlashString& reference to it,
which is declared static for the local version.

Names will be C++ decorated and adopt any namespace within which they are declared or defined.

Using FlashStrings
------------------

You can generally use a FlashString anywhere you can use a regular String as it has
an implicit *String()* operator.

To load or access the data as if it were a C array, use ``LOAD_FSTR`` (see below).

To read parts of a FlashString, use the ``read()`` method.

If the data isn't used very often, use ``readFlash()`` method instead as it avoids
disrupting the CPU cache. The ``FlashMemoryStream`` class does this by default.

Importing files
---------------

If you want to import data from a file, do this::

   IMPORT_FSTR(myData, PROJECT_DIR "/files/myData.bin");

Note the use of :envvar:`PROJECT_DIR` which is required to ensure the file path is absolute,
otherwise the compiler won't be able to find the file.

If you're building a Component, do this::

   IMPORT_FSTR(myFlashString, COMPONENT_PATH "/files/myFlashData.bin");

This will also work when used within a project.

A single global C symbol *myFlashString* will be defined so take care
with naming to avoid the risk of conflicts, especially if used within a component.

One use for importing files like this is to stream the data, like this::

   void onFile(HttpRequest& request, HttpResponse& response)
   {
      Serial.printf("myData is %u bytes long\n", myData.length());
      auto fs = new FlashMemoryStream(myData);
      response.sendDataStream(fs);
   }

This idea is extended further using :doc:`FlashStringMap`.


Additional Macros
-----------------

LOAD_FSTR(name, fstr)
   Load a FlashString object into a named local (stack) buffer.
   Faster than using a temporary String and avoids using the heap::

      DEFINE_FSTR(globalTest, "This is a testing string");

      void func()
      {
         LOAD_FSTR(local, globalTest);
         printf("%s, %u characters, buffer is %u bytes\n", local, globalTest.length(), sizeof(local));
      }

FSTR_ARRAY(name, str)
   Define a flash string and load it into a named char[] buffer on the stack. This::

      FSTR_ARRAY(buffer, "text");

   Is roughly equivalent to::

      char name[] = "text";

   Except the buffer is word aligned, so *sizeof(name)* may differ.

FSTR_PTR(data_ptr)
   Cast a custom structure to ``const FlashString*``.

DEFINE_FSTR_REF(name, data)
   Define a ``const FlashString&`` reference to a custom structure

DEFINE_FSTR_DATA(name, str)
   Define a FlashString-data structure without an associated reference::
   
      DEFINE_FSTR_DATA(myData, "hello");
      Serial.println(myData.fstr);

DEFINE_FSTR_DATA_LOCAL(name, str)
   Use for local (static) definitions.

IMPORT_FSTR_DATA(name, file)
   Use this to import the contents of a file without define a FlashString variable.
   You'll need to define your own symbol, remember the first word is the length::

      IMPORT_FSTR_DATA(myCustomData, PROJECT_DIR "/files/data.bin");
      struct MyCustomStruct {
         uint32_t length;
         char name[12];
         char description[256];
         uint8_t data[1024];
      };
      extern "C" const MyCustomStruct myCustomData;

