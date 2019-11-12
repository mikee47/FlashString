FlashString Library
===================

.. highlight:: C++

Introduction
------------

Perhaps the most common use for PROGMEM data is passing strings around, usually as a
pointer to a 'C' NUL-terminated char array. There are a couple of problems with this:

1. If we need to know how long the string is, we need to call *strlen_P*, which is
   _really_ expensive computationally.
2. We can't include NUL characters in the string.

Both of these are easily solved by passing the length along with the string, like this::

   char myString[] = "hello, this is a string";
   Serial.println(myString, sizeof(myString) - 1);

Of course, passing two parameters instead of one gets tiresome and is not very C++, is it?


What is a FlashString?
----------------------

A FlashString is a counted String object, stored in flash.

That is to say, the class instances themselves are actually stored in PROGMEM.

There are a few caveats to this:

1. No constructors
2. No virtual methods
3. Member data must be initialised at compile time

It's not magic, so using them from interrupt service routines is still forbidden. Sorry.

This library contains several classes, with macros to simplify data construction.
Here are the guides to using the library. For details of the class methods, see the source code.

.. toctree::

   FlashString
   FlashMemoryStream
   FlashStringTable
   FlashStringMap
   FlashStringIntMap
   technical
   upgrading
