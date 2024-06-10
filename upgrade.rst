Upgrade notes
=============

.. highlight:: c++

Version 1.0
-----------

FlashString was first introduced in Sming 3.7.0 on 17 November 2018, as a single file in the Wiring directory.
Other than bugfixes and Host Emulator support it hasn't had any significant updates.

Version 2.0
-----------

This library was introduced to Sming in version 4.0.1.
If you are migrating from Sming 3.7.0 or later and have used FlashString in your projects,
some minor changes may be necessary.

Version 2.1
-----------

Support for copied strings was removed to improve portability and performance.
Code simplification allowed use of compilers other than GCC, such as Clang/LLVM.

Code impact is minimal since FlashString objects should be passed by reference or pointer.


FlashString
~~~~~~~~~~~

This has been moved inside the ``FSTR`` namespace and renamed to ``String``.

FlashString has been retained as an alias for convenience to avoid ambiguity when used with Wiring String.

``FlashString::isEqual()`` has been renamed to ``equal()`` for consistency with Wiring String. 

Tables
------

Table support has been improved and formalised using the ``Vector`` class. Previously::

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

   DEFINE_FSTR_LOCAL(str1, "Test string #1");
   DEFINE_FSTR_LOCAL(str2, "Test string #2");

   DEFINE_FSTR_VECTOR(table, FlashString, &str1, &str2);

   Serial.printf("FSTR table[%u]\n", table.length());
   Serial.printf(" fstr1 = '%s'\n", String(table[0]).c_str());
   Serial.printf(" fstr1.length() = %u\n", table[0].length());

And perform lookups:

   Serial.print(" indexOf('Test STRING #1') = ");
   Serial.println(table.indexOf("Test STRING #1"));


Maps and other features
-----------------------

Associative maps have been added to support keys using a Flash String or an integral type.
Content is typically a String, but can also be another Table or Map for building hierarchical structures.

Moving to class templates has added a lot of possibilities so I hope you
have fun finding out what can be done with this library!

Suggestions for improvements and fixes always welcome :-)
