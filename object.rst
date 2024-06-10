Objects
=======

.. highlight:: C++

Introduction
------------

An :cpp:class:`FSTR::Object` is a class template with array-like behaviour, though it is not used directly.

Instead, use one of the four classes in the library:

-  :doc:`String <string>`
-  :doc:`Array <array>`
-  :doc:`Vector <vector>`
-  :doc:`Map <map>`

Each type has its own set of macros for easy data construction, and creation of the
appropriate Object class which may then be used directly.

Macros follow the same pattern:

``DEFINE_FSTR_*``
   Creates a static data structure with an associated Object reference.
   The _LOCAL variant makes the reference ``static constexpr``.

``DECLARE_FSTR_*``
   Use this in a header to declare an Object reference so it can be used across
   translation units.

Created symbols are C++ and adopt any enclosing namespaced.


Reading Object content
----------------------

To read parts of an Object, use the :cpp:func:`FSTR::Object::read` method.

If the data isn't used very often, use the :cpp:func:`FSTR::Object::readFlash` method instead as it avoids
disrupting the cache. The :cpp:class:`FSTR::Stream` class (alias :cpp:type:`FlashMemoryStream`) does this by default.


Object Internals
----------------

This section provides some examples of how structures are created, but in normal use you
should use the provided macros as they simplify the task and include structure validity checks.

:cpp:class:`FSTR::ObjectBase` is a non-template
`POD <https://stackoverflow.com/questions/4178175/what-are-aggregates-and-pods-and-how-why-are-they-special/7189821>`__
base class, and looks like this (methods omitted)::

   class ObjectBase {
      uint32_t flashLength_;
      // uint8_t data[];
   };

.. attention::

   ``flashLength_`` must not be accessed directly; use the ``length()`` method instead.

Data structures are created using, for example, ``DEFINE_FSTR(helloData, "hello")``.
This generates the following layout::

   constexpr const struct {
      FSTR::String object;
      char data[8];
   } __fstr__helloData PROGMEM = {
      {5},
      "hello"
   };
   const FSTR::String& helloData PROGMEM = __fstr__helloData.object;

.. note::

   The ``__fstr__`` prefix ensures that these structures are stored in flash memory on the esp8266.
   When templates are involved the ``PROGMEM`` segment attribute gets discarded.

   Do not access ``__fstr__helloData`` directly, it may change in future library versions.

References are an efficient and convenient way to access an Object, and should not consume
any memory themselves as the compiler/linker resolves them to the actual object.

However, in practice the Espressif compiler stores a full pointer to most things to support
relative addressing, and if the references aren't declared PROGMEM they'll consume RAM.

Objects may be cast to a reference of another required type, like this::

   auto& arr = helloData.as<FSTR::Array<char>>();


Copy behaviour
--------------

Objects cannot be created dynamically, nor can they be copied.
Always pass by reference.


Aggregate initialization
------------------------

We use `aggregate initialization <https://en.cppreference.com/w/cpp/language/aggregate_initialization>`__
to set up the structures so the data is fixed at link time without any constructor or initialiser functions.

This means classes cannot have:

-  user-provided constructors
-  brace-or-equal-initializers for non-static data members
-  private or protected non-static data members
-  virtual functions
-  base classes (until C++17)

This is why objects have no constructors or assignment operators.


Structure checks
----------------

The construction macros include a sanity check to ensure the initialization is
truly just Plain Old Data, without any hidden initialisers.

You may encounter one of the following errors during compilation:

-  The value of 'X' is not usable in a constant expression
-  FSTR structure not POD

This generally means one or more of the arguments in the initialisation data is not ``constexpr``.

In testing, this happens with references for global Objects, which of course cannot be constexpr.
To fix it, the offending Object needs to be redefined LOCAL.


Macros
------

.. doxygengroup:: fstr_object
   :content-only:

Class Template
--------------

.. doxygenclass:: FSTR::Object
   :members:
