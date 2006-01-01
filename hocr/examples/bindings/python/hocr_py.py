#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
from hocr import *

# set pic path if given by user
if len(sys.argv) > 1:
    pic = sys.argv[1]
    hocr_engien = Hocr (pic)
    hocr_engien.set_opt_n (0) # turn nikud off
    print hocr_engien.do_ocr ()
else:
    print "USAGE: hocr_py path_to_pnm_picture"