FlashString IntMap
==================

.. highlight:: C++

This is used to define associative mappings between an Integral key and a FlashString, *key => content*.

It is implemented as a class template so an additional template parameter *TKey* is required to indicate the
type used for the key. Otherwise it's very similar to :doc:`FlashStringMap`.

Each entry in the map consists of a *FlashStringIntPair<>*::

   struct FlashStringIntPair<TKey> {
      const TKey key_;
      const FlashString* content_;
   };

The *FlashStringIntMap* itself looks like this, without methods::

   struct FlashStringIntMap<TKey> {
      uint32_t mapLength;
      // FlashStringIntPair<TKey> values[];
   };

And here's how it's used::

   #include <FlashStringIntMap.h>

   IMPORT_FSTR(content1, PROJECT_DIR "/files/index.html");
   IMPORT_FSTR(content2, PROJECT_DIR "/files/favicon.html");

   const struct {
      FlashStringIntMap<int> map;
      const FlashStringIntPair<int> data[2];
   } flashMapData PROGMEM = {
      {2},
      {35, &content1},
      {180, &content2},
   };
   const auto& intmap = flashMapData.map;

Using a macro::

   DEFINE_FSTR_INTMAP(intmap, int, {35, &content1}, {180, &content2} );

We can now do this::

   void getValue(int key)
   {
      auto value = intmap[key];
      if(value) {
         Serial.printf("Found '%u' in map, containing %u chars\n", value.key(), value.content().length());
      } else {
         Serial.printf("Couldn't find '%u' in map\n", key);
      }
   }


Additional Macros
-----------------

DEFINE_FSTR_INTMAP_DATA(name, TKey, ...)
   Define the map structure without an associated reference.

FSTR_INTMAP_PTR(data_ptr, TKey)
   Cast a custom structure to a ``const FlashStringIntMap<TKey>*``.

DEFINE_FSTR_INTMAP_REF(name, TKey, data_name)
   Define a ``const FlashStringIntMap<TKey>&`` reference to a custom structure

