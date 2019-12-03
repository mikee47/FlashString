Vectors
=======

.. highlight:: c++

Introduction
------------

A :cpp:class:`FSTR::Vector` is an array of Object pointers::

   struct Vector<ObjectType> {
      FSTR::Object object;
      ObjectType* entries[];
   };

A key use for this is the construction of string tables.

Defining Vectors
----------------

Inline Strings are not supported, so the content has to be defined first::

   DEFINE_FSTR(str1, "Test string #1");
   DEFINE_FSTR(str2, "Test string #2");
   IMPORT_FSTR(str3, PROJECT_DIR "/files/somedata.json");

Now we can define the Vector::

   #include <FlashString/Vector.hpp>

   DEFINE_FSTR_VECTOR(myTable, FlashString,
      &str1,
      &str2,
      nullptr,
      &str3
   );

Note the use of ``nullptr`` to indicate an invalid vector entry, as distinct from an empty String.

Using Vectors
-------------

Now we can access the data using Vector methods::

   debugf("table.length() = %u", table.length());
   debugf("fstr1 = '%s'", String(table[0]).c_str());
   debugf("fstr2.length() = %u", table[1].length());
   debugf("fstr3.length() = %u", table[2].length());

You can share Vectors between translation units by declaring it in a header::

   DECLARE_FSTR_VECTOR(table);

To search a Vector::

   int i = table.indexOf("TEST STRING #1");

.. note::

   By default, searches in Vector<String> are not case-sensitive.

   The ``indexOf`` method has an extra ``ignoreCase`` parameter, which defaults to ``true``.


Structure
---------

The above example generates a structure like this::

   const struct {
      ObjectBase object;
      String* entries[4];
   } __fstr__myTable PROGMEM = {
      {16},
      &str1,
      &str2,
      nullptr,
      &str3,
   };
   const Vector<String>& myTable PROGMEM = __fstr__myTable.as<Vector<String>>();

Note: ``FSTR::`` namespace qualifier omitted for clarity.


Macros
------

.. doxygengroup:: fstr_vector
   :content-only:

Class Template
--------------

.. doxygenclass:: FSTR::Vector
