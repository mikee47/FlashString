Objects
=======

.. highlight:: C++

Introduction
------------

An ``Object`` is a class template with array-like behaviour and is used to implement
the four classes in the library:

-  String
-  Array
-  Vector
-  Map

Each type has its own set of macros for easy data construction, and creation of the
appropriate Object class which may then be used directly.

Macros follow the same pattern:

DEFINE_FSTR_\*
   Creates a static data structure with an associated Object reference.
   The _LOCAL variant makes the reference static.

DECLARE_FSTR_\*
   Use this in a header to declare Object reference so it can be used across
   translation units.

Created symbols are C++ and adopt any enclosing namespaced.

To read parts of a FlashString, use the ``read()`` method.

If the data isn't used very often, use the ``readFlash()`` method instead as it avoids
disrupting the cache. The ``Stream`` class (alias FlashMemoryStream) does this by default.


Object Internals
----------------

``ObjectBase`` is a non-template
`POD <https://stackoverflow.com/questions/4178175/what-are-aggregates-and-pods-and-how-why-are-they-special/7189821>`__
base class, and looks like this (methods omitted)::

   class ObjectBase {
      uint32_t flashLength_;
      // uint8_t data[];
   };

.. attention::

   ``flashLength_`` must not be accessed directly; use the ``length()`` method instead.

Data structures are created like this::

   constexpr const struct {
      ObjectBase object;
      char data[8];
   } flashHelloData PROGMEM = {
      {5},
      "hello"
   };

The ``object`` field may then be cast to the required type, like this::

   auto& str = flashHelloData.object.as<FSTR::String>();

If you want to access it as an array, do this::

   auto& arr = str.as<FSTR::Array<char>>();

Copy behaviour
--------------

To permit aggregate initialization, ObjectBase cannot have any user-defined constructors.
This means it cannot be copied directly.

.. note::

   Don't try to copy ObjectBase::

      auto myCopy = flashHelloData.object;
      Serial.print("myCopy.length() = ");
      Serial.println(myCopy.length());

   In debug builds, this will throw an assertion. In release builds, you'll get a zero-length object.

This is why we cast to an Object type, which adds the necessary copy constructors::

      auto myCopy = flashHelloData.object.as<FSTR::String>();
      Serial.print("myCopy.length() = ");
      Serial.println(myCopy.length());


Aggregate initialization
------------------------

We use `aggregate initialization <https://en.cppreference.com/w/cpp/language/aggregate_initialization>`__
to set up the structures so the data is fixed at link time without any constructor or initialiser functions.

This means classes cannot have::

-  user-provided constructors
-  brace-or-equal-initializers for non-static data members
-  private or protected non-static data members
-  virtual functions
-  base classes (until C++17)

This is why we use ObjectBase.
