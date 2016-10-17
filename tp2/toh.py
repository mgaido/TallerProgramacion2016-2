#!/usr/bin/python
print("static const unsigned char IMAGE_DATA[] = {{{}}};".format(
        ",".join(hex(ord(b)) for b in open("missing.png", "rb").read())))
