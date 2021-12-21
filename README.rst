FlashString
===========

.. highlight:: C++

Introduction
------------

This is a C++ library to simplify the definition and use of data structures stored in
program (flash) memory in an embedded microcontroller.

It was developed for use with `Sming <https://github.com/SmingHub/Sming>`__ and the ESP8266
but could be ported to other platforms relatively easily.

Perhaps the most common use for PROGMEM data is passing strings around, usually as a
pointer to a 'C' NUL-terminated char array. There are a couple of problems with this:

1. If we need to know how long the string is, we need to call :c:func:`strlen_P`, which is
   *really* expensive computationally.
2. We can't include NUL characters in the string.

Both of these are easily solved by passing the length along with the string, like this::

   char myString[] PROGMEM = "hello, this is a string";
   Serial.println(FPSTR(myString), sizeof(myString) - 1);

Of course, passing two parameters instead of one gets tiresome and is not very C++, is it?

This library implements C++ objects stored in flash memory, using macros to create the
data structures. The object interfaces are implemented using class templates for
performance and flexibility.

The classes are all in the ``FSTR`` namespace.

.. toctree::
   :maxdepth: 1
   
   object
   string
   array
   table
   vector
   map
   streams
   utility


Sming Integration
-----------------

Sming provides several aliases to provide compatibility with existing code:

-  :cpp:type:`FlashString` -> :cpp:class:`FSTR::String`
-  :cpp:type:`FlashMemoryStream` -> :cpp:class:`FSTR::Stream`
-  :cpp:type:`TemplateFlashMemoryStream` -> :cpp:class:`FSTR::TemplateStream`


Other pages
-----------

.. toctree::
   :maxdepth: 1
   
   upgrade
   changelog
   technical
   todo
   about
