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

The FlashStrings must be defined separately, for example::

   DEFINE_FSTR_DATA(fstr1, "Test string #1");
   DEFINE_FSTR_DATA(fstr2, "Test string #2");
   IMPORT_FSTR_DATA(fstr3, PROJECT_DIR "/files/somedata.json");

You can define a structure like this::

   #include <FlashStringTable.h>

   const struct {
      FlashStringTable table;
      const FlashString* entries[3];
   } flashTableData PROGMEM = { {3}, &fstr1, &fstr2, &fstr3 };

But of course it's easier using the macros::

   DEFINE_FSTR_TABLE(table, &fstr1, &fstr2, &fstr3);

And access the data using table methods::

   debugf("table.length() = %u", table.length());
   debugf("fstr1 = '%s'", String(table[0]).c_str());
   debugf("fstr2.length() = %u", table[1].length());
   debugf("fstr3.length() = %u", table[2].length());


Additional Macros
-----------------

FSTR_TABLE_PTR(data_ptr)
   Cast a custom structure to ``const FlashStringTable*``.

DEFINE_FSTR_TABLE_REF(name, data_name)
   Define a ``const FlashStringTable&`` reference to a custom structure

