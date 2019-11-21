Importing files
===============

For String and Array objects you can import data directly from a file
using IMPORT_FSTR() or IMPORT_FSTR_ARRAY(). For example::

   IMPORT_FSTR(myData, PROJECT_DIR "/files/myData.bin");

.. note::

   File paths must be absolute or the compiler won't be able to locate it reliably.

   Sming provides :envvar:`PROJECT_DIR` and :envvar:`COMPONENT_PATH` to help with this.

A single global C symbol ``myFlashString`` will be defined so take care
with naming to avoid the risk of conflicts, especially if used within a Component.

One use for imported files is to serve content via HTTP, like this::

   void onFile(HttpRequest& request, HttpResponse& response)
   {
      Serial.printf("myData is %u bytes long\n", myData.length());
      auto fs = new FSTR::Stream(myData);
      response.sendDataStream(fs);
   }

Therefore files can be bound into the firmware and accessed without requiring a filing system.
This idea is extended further using :doc:`map`.


Additional Macros
-----------------

IMPORT_FSTR_DATA(name, file)
   Import the contents of a file without defining any C/C++ variable::

      IMPORT_FSTR_DATA(myCustomData, PROJECT_DIR "/files/data.bin");

   You'll need to define an appropriate symbol::

      struct MyCustomStruct {
         uint32_t length;
         char name[12];
         char description[256];
         uint8_t data[1024];
      };

      extern "C" const MyCustomStruct myCustomData;

   You'll still have to consider how the data is accessed. If it's small and un-complicated
   you can just copy it into RAM::

      MyCustomStruct buf;
      memcpy_P(&buf, &myCustomData, sizeof(buf));

   However, a better way is to define a custom Object to handle it.
   You can find an example of how to do this in ``test/app/custom.cpp``.
