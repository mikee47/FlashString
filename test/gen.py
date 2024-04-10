#!/usr/bin/env python3
#
# Test data generation
#

import random
import os

print('DEFINE_FSTR_ARRAY(largeIntArray, int,', ",".join(str(i*123) for i in range(1000)), ')')
print()

filename = os.path.expandvars('${SMING_HOME}/README.rst')
words = open(filename).read().split()

print('#define LARGE_STRING_MAP(XX)', " \\\n  ".join(f'XX({i}, "{s}")' for i, s in enumerate(words)))
print()
