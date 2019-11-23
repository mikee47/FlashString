Arrays
======

.. highlight:: c++

Introduction
------------

Supports arrays of simple types, such as char, int, double, or POD structures (i.e. basic C structures).

:cpp:class:`FSTR::Array` is a class template, so requires an additional ``ElementType`` parameter::

   #include <FlashString/Array.hpp>

   DEFINE_FSTR_ARRAY(myDoubleArray, double,
      PI, 53.0, 100, 1e8, 47
   );
   Serial.print("My double array: ");
   myDoubleArray.printTo(Serial);
   Serial.println();


.. note::

   Objects do not inherit from Printable because it is a virtual base class.

   Therefore, statements like ``Serial.println(myDoubleArray)`` are not supported.

   This also avoids ambiguity between implicit WString conversions.

There are some Print helper functions in the library you can use::

   FSTR::println(Serial, myDoubleArray);

These are templated so will handle both simple data types and Objects.

You can share Arrays between translation units by declaring it in a header::

   DECLARE_FSTR_ARRAY(table);


Macros
------

.. doxygengroup:: fstr_array
   :content-only:

Classes
-------

.. doxygenclass:: FSTR::Array
