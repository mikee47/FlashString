FlashString Map
===============

.. highlight:: C++

This is used to define associative mappings between two FlashStrings, *key => content*.

It's analogous to *HashMap<FlashString, FlashString>*, except it doesn't use any RAM
and cannot be updated at runtime.

Each entry in the map consists of a *FlashStringPair*, containing pointers to two FlashStrings::

   struct FlashStringPair {
      const FlashString* key_;
      const FlashString* content_;
   };

The *FlashStringMap* itself looks like this, without methods::

   struct FlashStringMap {
      uint32_t mapLength;
      // FlashStringPair values[];
   };

And here's how it's used::

   #include <FlashStringMap.h>

   DEFINE_FSTR_DATA_LOCAL(key1, "index.html");
   DEFINE_FSTR_DATA_LOCAL(key2, "favicon.ico");
   IMPORT_FSTR(content1, PROJECT_DIR "/files/index.html");
   IMPORT_FSTR(content2, PROJECT_DIR "/files/favicon.html");

   const struct {
      FlashStringMap map;
      const FlashStringPair data[2];
   } flashMapData PROGMEM = { {2}, { &key1.fstr, &content1}, { &key2.fstr, &content2 } };
   const auto& fileMap = flashMapData.map;

Or, using a macro::

   DEFINE_FSTR_MAP(fileMap, { &key1.fstr, &content1}, {&key2.fstr, &content2} );

We can now do this::

   void onFile(HttpRequest& request, HttpResponse& response)
   {
      String fileName = request.uri.getRelativePath();
      auto& value = fileMap[fileName];
      if(value) {
         // Found
         Serial.printf("Found '%s' in fileMap\n", String(value.key()).c_str());
         auto stream = new FlashMemoryStream(value);
         response.sendDataStream(stream, ContentType::fromFullFileName(fileName));
      } else {
         Serial.printf("File '%s' not found\n", fileName.c_str());
      }
   }


Additional Macros
-----------------

DEFINE_FSTR_MAP_DATA(name, ...)
   Define the map structure without an associated reference.

FSTR_MAP_PTR(data_ptr)
   Cast a custom structure to a ``const FlashStringMap*``.

DEFINE_FSTR_MAP_REF(name, data_name)
   Define a ``const FlashStringMap&`` reference to a custom structure

