FlashMemoryStream
-----------------

This is an *IDataSourceStream* descendant which you can use to stream the contents of
a FlashString. It's especially useful when used in conjuction with IMPORT_FSTR::

   IMPORT_FSTR(myLargeFile, PROJECT_DIR "/files/lots-of-stuff.txt");
   FlashMemoryStream fs(myLargeFile);
   Serial.println(myLargefile);

Because the data is read in sections, it's not limited by available RAM.

Like a FileStream, you can also seek randomly within a *FlashMemoryStream*, so you can
use it as the basis for an elementary read-only filesystem.
See also :doc:`FlashStringMap` for an example.
