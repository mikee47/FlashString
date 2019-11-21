Arrays
======

.. highlight:: c++

Introduction
------------

Supports arrays of simple types, such as char, int, double, or POD structures (i.e. basic C structures).

The ``Array`` is a class template, so requires an additional ``ElementType`` parameter::

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


Tables
------

Simple tables can be implemented using Arrays, like this::

   struct TableRow {
      float columns[3];
      
      int operator[](size_t index) const
      {
         return columns[index];
      } 
   };
   DEFINE_FSTR_ARRAY(table, TableRow,
      {0.1, 0.2, 0.3},
      {0.6, 0.7, 0.8}
   );
   for(auto row: table) {
      Serial.printf("%f, %f, %f\n", row[0], row[1], row[2]);
   }

Each row is a fixed size. The ``TableRow`` class template is provided to simplify this::

   #include <FlashString/Table.hpp>

   using FloatRow = FSTR::TableRow<float, 3>;
   DEFINE_FSTR_ARRAY(table, FloatRow,
      {0.1, 0.2, 0.3},
      {0.6, 0.7, 0.8}
   );
   table.printTo(Serial);
   table.println();


If you want to create a table with rows of different sizes or types, use a :doc:`Vector <vector>`.


Additional Macros
-----------------

DEFINE_FSTR_ARRAY_DATA(name, ...)
   Define the data structure without an associated reference.
