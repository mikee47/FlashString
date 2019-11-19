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


Reading Object content
----------------------

To read parts of an Object, use the ``read()`` method.

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

The ``object`` field may then be cast to a reference of the required type, like this::

   auto& str = flashHelloData.object.as<FSTR::String>();

If you want to access it as an array, do this::

   auto& arr = str.as<FSTR::Array<char>>();

References are an efficient and convenient way to access an Object, and should not consume
any memory themselves as the compiler/linker resolve them to the actual object.

However, in practice the Espressif compiler stores a full pointer to most things and if
the references aren't declared PROGMEM they'll consume RAM.


Copy behaviour
--------------

Whilst references are the preferred way to access flash Objects, they can also be created dynamically::

   FSTR::String emptyString;
   FSTR::String stringCopy(FS("Inline string"));

Such instances are stored in RAM but only consume 4 bytes as they simply keep a pointer
to the real flash Object.

.. note::

   Don't try to copy ObjectBase!
   
Here's a somewhat contrived example to demonstrate::

   DEFINE_FSTR_DATA_LOCAL(flashHelloData, "Hello");
   auto myCopy = flashHelloData.object;
   Serial.print("myCopy.length() = ");
   Serial.println(myCopy.length());

In debug builds, this will throw an assertion. In release builds, you'll get a zero-length object.

To permit aggregate initialization, ObjectBase cannot have any user-defined constructors
which is why we always cast to an Object reference which has the necessary copy constructors::

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


Structure checks
----------------

The FlashString construction macros all include a sanity check to ensure the initialization is
truly just Plain Old Data, without any hidden initialisers.

You may encounter one of the following errors during compilation:

-  The value of 'X' is not usable in a constant expression
-  FSTR structure not POD

This generally means one or more of the arguments in the initialisation data is not ``constexpr``.
Most compilers are quite relaxed about this but ``GCC 4.8.5`` is particularly thick.

In testing, this happens with references for global Objects, which of course cannot be constexpr.
To fix it, the offending Object either needs to be redefined LOCAL, or if the Object data is in
scope (i.e. defined in the same source file) then you can get a direct pointer to it using
the ``FSTR_PTR`` macro.
