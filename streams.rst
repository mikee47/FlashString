Streams
=======

.. cpp:class:: FSTR::Stream : public IDataSourceStream

Alias: :cpp:type:`FlashMemoryStream`

This is a Sming :cpp:class:`IDataSourceStream` descendant which you can use to stream the contents of
any FlashString object. It's especially useful when used with :c:macro:`IMPORT_FSTR`::

   IMPORT_FSTR(myLargeFile, PROJECT_DIR "/files/lots-of-stuff.txt");
   FSTR::Stream fs(myLargeFile);
   Serial.println(myLargefile);

Because the data is read in sections, it's not limited by available RAM.

.. note::
   
   Unless you need ``myLargeFile`` to be a global symbol, you'll generally want to use :c:func:`IMPORT_FSTR_LOCAL`.

Like a :cpp:class:`FileStream`, you can also seek randomly within a :cpp:class:`FSTR::Stream`,
so you can use it as the basis for an elementary read-only filesystem.

See :doc:`map` for a more useful example.

.. cpp:class:: FSTR::TemplateStream : public TemplateStream

Alias: :cpp:type:`TemplateFlashMemoryStream`

Standard templating stream for tag replacement.

