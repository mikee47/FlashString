FlashString Tables
==================

.. highlight:: c++

A common requirement is to define a table of strings and access them by index.

This is easily handled using a *FlashStringTable* object, similar to *Vector<String>*
except it doesn't use any RAM and cannot be updated at runtime.

Here it is without the methods::

   struct FlashStringTable {
      uint32_t tableLength;
      // FlashString* entries[];
   };

The FlashStrings must be defined first::

   DEFINE_FSTR_DATA(data1, "Test string #1");
   DEFINE_FSTR_DATA(data2, "Test string #2");
   IMPORT_FSTR(fstr3, PROJECT_DIR "/files/somedata.json");

.. attention::

   Take care to use DEFINE_FSTR_DATA, not DEFINE_FSTR.

   This is because we need the actual FlashString and not a reference to it.

   IMPORT_FSTR is fine because it creates an actual FlashString object, not a reference.

Then you can define the table structure::

   #include <FlashStringTable.h>

   const struct {
      FlashStringTable table;
      const FlashString* entries[3];
   } flashTableData PROGMEM = {
      {3},
      &data1.fstr,
      &data2.fstr,
      &fstr3,
   };

.. attention::

   If you try to use FlashString& references, the code will compile (and will work
   on the Host Emulator), but will fail to start on a real device.

   This is because startup constructor functions are created to copy the
   references into the structure. It cannot do this because they are in PROGMEM.


But of course it's easier using a macro::

   DEFINE_FSTR_TABLE(table, &data1.fstr, &data2.fstr, &fstr3 );

Now we can access the data using table methods::

   debugf("table.length() = %u", table.length());
   debugf("fstr1 = '%s'", String(table[0]).c_str());
   debugf("fstr2.length() = %u", table[1].length());
   debugf("fstr3.length() = %u", table[2].length());

As with FlashString, you can share tables between translation units by declaring it in a header::

   DECLARE_FSTR_TABLE(table);


Additional Macros
-----------------

DEFINE_FSTR_TABLE_DATA(name, ...)
   Define the table structure without an associated reference.

FSTR_TABLE_PTR(data_ptr)
   Cast a custom structure to ``const FlashStringTable*``.

DEFINE_FSTR_TABLE_REF(name, data_name)
   Define a ``const FlashStringTable&`` reference to a custom structure

