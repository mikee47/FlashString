About FlashString
=================

FlashString was first introduced in Sming 3.7.0 on 17 November 2018.

I created it as a one-file solution to address these specific issues:

1. Regular Strings are really useful for passing around any kind of heap-allocated data,
   but it's limited by available RAM.

   The https://github.com/mikalhart/Flash library for Arduino provides a good approach,
   creating a wrapper class around the flash data. But I wanted it to be able to treat my
   FlashString classes as if they were just stored in flash so I wouldn't have to wrap
   everything in a macro, like F() does.

   Solution: The FlashString class.

2. Using PROGMEM directly for data is cumbersome, slow and fraught with danger: It's inherently unsafe
   because of the alignment issues. Some smart cookie came up with a compiler patch so it could
   insert the correct instructions and thus avoid alignment exceptions. However, that still causes
   execution of inefficient code since the hardware requires we perform aligned accesses.
   Generating the structures correctly in the first place felt the best way forward.

   Solution: DEFINE_FSTR

3. Sharing flash data structures globally

   Solution: DECLARE_FSTR

4. How to get content into PROGMEM without having to manually convert everything into
   C structures. One solution to this is using external tools but that complicates the build
   process and felt un-necessary.
   
   Solution: IMPORT_FSTR

5. Relying on SPIFFS for serving fixed content is inefficient and problematic if/when the
   filesystem gets corrupted. I needed a solution which allowed large content to be
   served up without requiring a filesystem. The long term solution to this is, of course,
   a read-only filesystem but that is a complex thing indeed to do properly.

   Solution: FlashMemoryStream


Embedded microsystems
---------------------

Back in the golden era of the Commodore 64 and the ZX81, *embedded* really meant something.
You had to get your hands dirty and poke about in the hardware.

With modern hardware it's cheating. I mean, how on earth can a basic Android 'App'
weigh in at 20MB? I cannot get my head around that.


Of course, it's just economics - far cheaper to throw lots of hardware resources at
a problem than actually build it right in the first place. With all that memory kicking
about it's just pure laziness. IMHO. And of course that trend will continue because
it feeds the hardware manufacturers in this disposable world.

Not a fan.

Anyway, before I lose the plot my point was that as the term 'embedded' has been
hijacked it needs at least some qualification.

So I'll say that this library, and my other work, is focused on embedded **microsystems** (EMS).


Why bother?
-----------

The main limitation of the ESP8266 is probably its RAM. So why not just use an ESP32?
It's got lots more RAM.

But it's still a finite resource, and I can think of far better ways to use it than
filling it up with constant data.

QED. RAM will **always** be a scarce commodity in an EMS.


Thanks
------

This library may have never been written if not for `ArduinoJson <https://arduinojson.org/>`__.
Of specific interest is the way class templates are used, the high quality of documentation,
ease of use and test-driven approach. I have attempted to emulate those particular
attributes here.

Special thanks to Slavey Karadzhov for his support and assistance over the past year
mentoring me through the process of upgrading the `Sming <https://github.com/SmingHub/Sming>`__
embedded framework. I've learned a phenomenal amount about modern development practices
and it's really given my coding practices a kick up the arse. Awesome :-)

