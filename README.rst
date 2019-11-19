FlashString Library
===================

.. highlight:: C++

Introduction
------------

This is a C++ library to simplify the definition and use of data structures stored in
program (flash) memory in an embedded microcontroller.

It was developed for use with `Sming <https://github.com/SmingHub/Sming>`__ and the ESP8266
but could be ported to other platforms relatively easily.

Perhaps the most common use for PROGMEM data is passing strings around, usually as a
pointer to a 'C' NUL-terminated char array. There are a couple of problems with this:

1. If we need to know how long the string is, we need to call *strlen_P*, which is
   _really_ expensive computationally.
2. We can't include NUL characters in the string.

Both of these are easily solved by passing the length along with the string, like this::

   char myString[] PROGMEM = "hello, this is a string";
   Serial.println(FPSTR(myString), sizeof(myString) - 1);

Of course, passing two parameters instead of one gets tiresome and is not very C++, is it?


The library
-----------

This library provides a set of class templates and macros for storing C++ objects directly
in flash memory.

Objects which may then be accessed using modern and efficient C++ programming techniques.

The classes are all in the ``FSTR`` namespace.

Note that ``FlashString``, ``FlashMemoryStream`` and ``TemplateFlashMemoryStream`` 
have been retained as aliases for compatibility with existing code.

.. toctree::

   object
   string
   array
   vector
   map
   utility
   upgrade
   changelog
   technical
   todo
   about
