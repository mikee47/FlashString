IMPORT_FSTR_DATA(name, file)
   Import the contents of a file without defining any C/C++ variable::

      IMPORT_FSTR_DATA(myCustomData, PROJECT_DIR "/files/data.bin");

   You'll need to define your own symbol::

      struct MyCustomStruct {
         uint32_t length;
         char name[12];
         char description[256];
         uint8_t data[1024];
      };
      extern "C" const MyCustomStruct myCustomData;
