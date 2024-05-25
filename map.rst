Maps
====

.. highlight:: C++

Introduction
------------

A :cpp:class:`FSTR::Map` is analogous to the Wiring :cpp:class:`HashMap` class, allowing content to be indexed using
a key value.

The Map contains an array of :cpp:class:`FSTR::MapPair` structures::

   struct MapPair<KeyType, ContentType> {
      KeyType key_;
      ContentType* content_;
   };

``KeyType`` can be any simple type such as ``char``, ``int``, ``float``, ``enum`` etc.
It may also be a ``String`` Object (or, more precisely, ``String*``).

``ContentType`` can be any Object type (String, Array, Vector or Map).
This allows hierarchical structures to be created.

.. |rArr| unicode:: 0x21D2 .. => arrow

Example: int |rArr| String
--------------------------

Here's a basic example using integer keys::

   #include <FlashString/Map.hpp>

   IMPORT_FSTR_LOCAL(content1, PROJECT_DIR "/files/index.html");
   IMPORT_FSTR_LOCAL(content2, PROJECT_DIR "/files/favicon.html");

   DEFINE_FSTR_MAP(intmap, int, FSTR::String,
      {35, &content1},
      {180, &content2}
   );

You should generally use :c:func:`IMPORT_FSTR_LOCAL` when referencing imported objects.
If you need global access to imported data as well, then use :c:func:`IMPORT_FSTR`.

We can now do this::

   void printValue(int key)
   {
      auto value = intmap[key];
      if(value) {
         Serial << "Found '" << value.key() << "' in map, containing " << value.content.length() << " chars" << endl;
         Serial << value << endl;
      } else {
         Serial << "Couldn't find '" << key << "' in map" << endl;
      }
   }


Example: String |rArr| String
-----------------------------

Both the key and the content are stored as Strings::

   #include <FlashString/Map.hpp>

   DEFINE_FSTR_LOCAL(key1, "index.html");
   DEFINE_FSTR_LOCAL(key2, "favicon.ico");
   IMPORT_FSTR_LOCAL(content1, PROJECT_DIR "/files/index.html");
   IMPORT_FSTR_LOCAL(content2, PROJECT_DIR "/files/favicon.html");

   DEFINE_FSTR_MAP(fileMap, FlashString, FlashString,
      {&key1, &content1},
      {&key2, &content2},
   );

We can now do this::

   void onFile(HttpRequest& request, HttpResponse& response)
   {
      String fileName = request.uri.getRelativePath();
      auto& value = fileMap[fileName];
      if(value) {
         // Found
         Serial << "Found '" << value.key() << "' in fileMap" << endl;
         auto stream = new FlashMemoryStream(value);
         response.sendDataStream(stream, ContentType::fromFullFileName(fileName));
      } else {
         Serial << "File '" << fileName << "' not found" << endl;
      }
   }

.. note::

   As with ``Vector<String>``, ``Map<String, ...>`` lookups are by default case-insensitive.
   
   If you require a case-sensitive lookup, use the ``indexOf`` method with ``ignoreCase = false``.


Structure
---------

The macro in the first example above produces a structure like this::

   constexpr const struct {
      ObjectBase object;
      MapPair<int, String> data[2];
   } __fstr__intmap PROGMEM = {
      {16},
      {35, &content1},
      {180, &content2},
   };
   const Map<int, String>& intmap = __fstr__intmap.object.as<Map<int, String>>();

Note: ``FSTR::`` namespace qualifier omitted for clarity.

Usually, each MapPair is 8 bytes, but if the key is a double or int64 it would be 12 bytes.


Macros
------

.. doxygengroup:: fstr_map
   :content-only:


Class Templates
---------------

.. doxygenclass:: FSTR::Map
   :members:

.. doxygenclass:: FSTR::MapPair
   :members:
