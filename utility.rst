Utilities
=========

Importing files
---------------

For String and Array objects you can import data directly from a file
using :c:func:`IMPORT_FSTR` or :c:func:`IMPORT_FSTR_ARRAY`. For example::

   IMPORT_FSTR(myData, PROJECT_DIR "/files/myData.bin");

This defines a C++ reference to the data called ``myData`` so it can be referred to using
:c:func:`DECLARE_FSTR` if required.

.. note::

   File paths must be absolute or the compiler won't be able to locate it reliably.

   Sming provides :envvar:`PROJECT_DIR` and :envvar:`COMPONENT_PATH` to help with this.


.. note::

   A corresponding global C symbol will also be defined, based on the provided name,
   to provide linkage with the imported data.

   Such symbols cannot be localised as it requires use of ``extern``, so take care with
   naming to avoid the risk of conflicts, especially if used within a Component.


One use for imported files is to serve content via HTTP, like this::

   void onFile(HttpRequest& request, HttpResponse& response)
   {
      Serial.printf("myData is %u bytes long\n", myData.length());
      auto fs = new FSTR::Stream(myData);
      response.sendDataStream(fs);
   }

Therefore files can be bound into the firmware and accessed without requiring a filing system.
This idea is extended further using :doc:`map`.


Custom Imports
--------------

Use :c:func:`IMPORT_FSTR_DATA` to import the contents of a file without defining any C/C++ variable::

      IMPORT_FSTR_DATA(myCustomData, PROJECT_DIR "/files/data.bin");

You'll need to define an appropriate symbol::

   struct MyCustomStruct {
      uint32_t length;
      char name[12];
      char description[20];
      uint8_t data[1024];
   };

   extern "C" const MyCustomStruct myCustomData;

You'll still have to consider how the data is accessed. If it's small and un-complicated
you can just copy it into RAM::

   MyCustomStruct buf;
   memcpy_P(&buf, &myCustomData, sizeof(buf));


Custom Objects
--------------

A better way to handle large, complex structures is to define a custom Object to handle it.
You can find an example of how to do this in ``test/app/custom.cpp``, which does this:

1. Define ``MyCustomStruct``::
   
      struct MyCustomStruct {
         FSTR::ObjectBase object;
         char name[12];
         char description[20];
         FSTR::ObjectBase dataArray;
      };

2. Define a base object type (``CustomObject``) using the :cpp:class:`FSTR::Object` class template.
   This determines the underlying element type, generally ``char`` or ``uint8_t`` are most useful.

3. Derive an Object class (``MyCustomObject``) to encapsulate access to ``MyCustomStruct``.

4. Use the :c:func:`IMPORT_FSTR_OBJECT` macro to import the custom data and define a global
   reference (``customObject``) of type ``MyCustomObject&``.

5. Use :c:func:`DECLARE_FSTR_OBJECT` macro to declare the reference in a header.
 
More complex examples may involve multiple custom Object types.


API Reference
-------------

.. doxygengroup:: fstr_utility
   :content-only:

.. doxygengroup:: fstr_print
   :content-only:
