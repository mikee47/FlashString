Tables
======

.. highlight:: c++

Introduction
------------

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

Each row is a fixed size. The :cpp:class:`FSTR::TableRow` class template is provided to simplify this::

   #include <FlashString/Table.hpp>

   using FloatRow = FSTR::TableRow<float, 3>;
   DEFINE_FSTR_ARRAY(table, FloatRow,
      {0.1, 0.2, 0.3},
      {0.6, 0.7, 0.8}
   );
   table.printTo(Serial);
   table.println();


If you want to create a table with rows of different sizes or types, use a :doc:`Vector <vector>`.


Class Template
--------------

.. doxygenclass:: FSTR::TableRow
