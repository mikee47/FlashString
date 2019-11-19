TODO List
=========

Change all sizes to size_t
   e.g. length()

Missing methods
   Behaviour of String should reflect WString. Review and add any missing methods.
   Note that implementations for some are likely non-trivial since we cannot assume
   the content will fit into RAM.

Benchmark filemap
   Compare SPIFFS vs. file map:
   
   - File access times
   - File transfer times

Implement stream operator <<
   For simpler printing. This is a large architectural decision as Sming doesn't have any of this,
   neither it seems does Arduino although some libraries add support for it.

   The advantage over Print/Printable is that support can be added using template functions
   without modifying the classes themselves. Formatting statements can be inserted to customise
   the output.


Formatted print output
   We have separate argument for Array printing, but if we want to customise the text for each item
   as well then we have to use a regular for-loop and handle the separators as well.

   Easiest is probably to just add format arguments to `printTo()` method.

   Simple example: print an array of integers in HEX.

   Need to consider with text escaping, probably leave that for external libraries.


Multi-dimensional arrays
   This could be an array of structs, so operator[] returns an entire row.
   As an optimisation, could we define additional accessors for nested levels?

   Alternatives:
   
   -  Vector<Array>. Each array has to be defined before table.
   -  Specialize operator for 2x2 case as it's quite common.
      e.g. SubElementType operator[](unsigned row, unsigned col) const

Type Information
   The flashLength_ value can be redefined like this::
      length: 20;
      elementSize: 3; ///< Number of bytes in each element, less one
      type: 5; ///< Enumeration identifying element type
         Char = 0,
         Signed,
         Unsigned,
         Float,
         Enum,
         String,
         Array,
         Vector,
         Map,
         UserDefined = 16 // Start of user-defined types

Variant Object
   Interprets objects based on type information at runtime.
   This would allow complex data structures to be defined and used without
   knowing the object types. For example, translated JSON or XML documents.

Translation tools
   Say we wanted to access a read-only JSON document. How might we do that?
   
   Easy. We write a Host application, so we can use all the existing libraries.
   Memory not an issue.

   1. Load the JSON document using ArduinoJson
   2. Serialize the document into FlashString-compatible declarations,
      producing a header file and a source file.

   All we need to do then is write a JSON serializer to produce the appropriate structures.


Sming improvements
------------------

memcmp_P
   Requires optimisation, comparing byte-by-byte is very slow.
   Look to libc implementations for ways to optimise.

