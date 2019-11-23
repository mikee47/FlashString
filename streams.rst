Streams
=======

.. cpp:class:: FSTR::Stream : public IDataSourceStream

Alias: :cpp:class:`FlashMemoryStream`

This is a Sming :cpp:class:`IDataSourceStream` descendant which you can use to stream the contents of
a FlashString. It's especially useful when used in conjuction with :c:macro:`IMPORT_FSTR`::

   IMPORT_FSTR(myLargeFile, PROJECT_DIR "/files/lots-of-stuff.txt");
   FSTR::Stream fs(myLargeFile);
   Serial.println(myLargefile);

Because the data is read in sections, it's not limited by available RAM.

Like a :cpp:class:`FileStream`, you can also seek randomly within a :cpp:class:`FlashMemoryStream`,
so you can use it as the basis for an elementary read-only filesystem.

See :doc:`map` for a more useful example.

.. cpp:class:: FSTR::TemplateStream : public TemplateStream

Alias: :cpp:class:`TemplateFlashMemoryStream`

Standard templating stream for tag replacement.

