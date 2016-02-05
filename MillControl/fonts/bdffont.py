# -*- coding: utf-8 -*-

import urllib, sys, os, io, math, platform, re
import Image, ImageDraw
from bdflib import reader, writer

if len(sys.argv) != 3 or sys.argv[2] not in ['numeric', 'symbol', 'regular']:
    print "Usage: fontname [numeric | symbol | regular]"
    print "fontname is as local filename or on https://github.com/olikraus/u8glib/tree/master/tools/font/bdf"
    sys.exit()

mode = sys.argv[2]

#-------------------------------------------------------------------------------
# font download

fontname = sys.argv[1]
filename = "%s.bdf" % fontname
if not os.path.isfile(filename):
    response = urllib.urlopen('https://raw.githubusercontent.com/olikraus/u8glib/master/tools/font/bdf/%s' % filename)
    src_bdf = response.read()
else:
    with open(filename, 'r') as bdf_file:
        src_bdf=bdf_file.read()
        src_bdf.close()

font = reader.read_bdf(iter(src_bdf.splitlines()))

def asc(s):
    return ord(s.decode('utf-8').encode('iso-8859-1'))

#-------------------------------------------------------------------------------
# copy one glyph over another

def replace(t, f):
    fr = font[asc(f)]
    to = font[asc(t)]
    #font.glyphs_by_codepoint[to] = font.glyphs_by_codepoint[fr]
    to.name = fr.name
    to.bbX = fr.bbX
    to.bbY = fr.bbY
    to.bbW = fr.bbW
    to.bbH = fr.bbH
    to.advance = fr.advance
    #to.codepoint = fr.codepoint
    to._set_data(fr.get_data())

#------------------------------------------------------------------------------
# dumps animage to bdf format

def image_to_glyph(name, im, glyph, template = None):
    to = font[asc(glyph)]

    define = "#define SYMBOL_%s %i\n" % (name, asc(glyph))

    if not template:
        template = glyph
    fr = font[asc(template)]

    hex_dump = [ "%02x" % ord(i) for i in im.tostring() ]

    #calc number of bytes per row
    parts = int(math.ceil(im.width / 8.0))

    #concat the rows
    rows = []
    for i in range(0, len(hex_dump), parts):
        rows.append("".join(hex_dump[i:i+parts]))

    #to.name = fr.name
    to.bbX = fr.bbX
    to.bbY = fr.bbY
    to.advance = im.width + (fr.advance - fr.bbW) #keeps the old spacing
    to.bbW = im.width
    to.bbH = im.height
    to._set_data(rows)

    return define

#-------------------------------------------------------------------------------
# special characters

if not mode == 'numeric':
    #add umlauts
    replace('[', 'ä')
    replace('\\', 'ö')
    replace(']', 'ü')
    replace('{', 'Ä')
    replace('|', 'Ö')
    replace('}', 'Ü')
    replace('~', 'ß')

#-------------------------------------------------------------------------------
# symbols

defines = ""
if mode == 'regular':
    replace('<', '(')
    replace('>', ')')
    replace('=', '!')
    replace('@', ' ')

if mode == 'symbol':
    replace('[', '(')
    replace(']', ')')

    #ok
    def ok(width):
        s = int(width * 0.3);
        height = width - s
        im = Image.new("1", (width, height), 0)
        draw = ImageDraw.Draw(im)
        draw.line([(0, height - s - 2), (s, height - 2), ((width - 1), (height - (width - s) - 1))], fill = 1, width = 2);
        return im

    defines += image_to_glyph('OK', ok(font[asc('A')].bbW), '_', 'A')

    #scale
    def scale(width, height):
        im = Image.new("1", (width, height), 0)
        draw = ImageDraw.Draw(im)
        s = int(im.width * 0.35);
        draw.ellipse([(0, im.height - 1 - s * 2), (im.width -1, im.height -1)], fill = 1, outline = 1)
        draw.rectangle([(0, im.height - 1 - s * 2), (im.width -1, im.height - 1 - s * 1)],  fill = 0)
        draw.line([(1, im.height - 1 - s), ((im.width / 2  - 1), 1), ((im.width - 2), im.height - 1 - s)], fill = 1, width = 2);
        return im

    t = font[asc('A')]
    defines += image_to_glyph('SCALE', scale(t.bbW, t.bbH), '\\','A')

    #back
    def back(h):
        w0 = ((h-1) / 2)
        w1 = 0
        w = w0 + 6
        im = Image.new("1", (w, h), 0)
        draw = ImageDraw.Draw(im)
        for l in range(0,2):
            for c in range(0,2):
                x1 = c + l * 4;
                draw.line([(x1 + w0, 0), (x1, w0), (x1 + w0, w0 * 2)], fill = 1)
        return im
    t = font[asc(':')]
    defines += image_to_glyph('BACK', back(t.bbH), '^','a')

#------------------------------------------------------------------------------
# write font
fontname = fontname + mode[0]
filename = "%s.bdf" % fontname
with io.open(filename, 'wb') as file:
    writer.write_bdf(font, file)
    file.close()

#------------------------------------------------------------------------------
# export to u8g

#load bdf2u8g
cmd_file = 'bdf2u8g.exe'
urllib.urlretrieve ('https://github.com/olikraus/u8glib/raw/master/tools/font/bdf2u8g/bdf2u8g.exe', cmd_file)

if platform.system() != 'Windows':
    cmd_file = 'wine ' + cmd_file

if mode == 'numeric':
    begin = asc('.')
    end = asc('9')
if mode == 'regular':
    begin = asc('.')
    end = asc('~')
if mode == 'symbol':
    begin = asc('+')
    end = asc('~')

defines += "#define %s_BEGIN %i\n" % (mode.upper(), begin)
defines += "#define %s_END %i\n" % (mode.upper(), end)

header_filename = "../%s.h" % fontname
cmd = cmd_file + " -b %i -e %i %s %s %s" % (begin, end, filename,  fontname, header_filename)
print cmd
os.system(cmd)

with open(header_filename, 'r') as header_file:
    headers=header_file.read()
    header_file.close()

headers = headers.replace('#include "u8g.h"','#include <U8glib.h>\n' + defines)
headername = "%s_H" % fontname.upper()
headers = "#ifndef %s\n#define %s\n" % (headername, headername) + headers + ("#endif //%s_H\n" % headername)

#endif //MILLCONTROL_RUN_H
with io.open(header_filename, 'wb') as header_file:
    header_file.write(headers)
    header_file.close()

#needs bdflib and urllib
#what fonts do i need?

#heading font
#numbers font
#symbol font
