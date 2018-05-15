from PIL import Image

MAP = "house"

im = Image.open("tiles.png")
pixels = list(im.getdata())

def extract_block(offsetX, offsetY):
    bytes = []
    b = 0
    for x in range(8):
        for y in range(8):
            pixel = pixels[im.width*(y+offsetY*8) + (x+offsetX*8)]
            if pixel == (0, 0, 0) or pixel == (0, 0, 0, 255):
                b = b | (128 >> y)

        bytes.append(b)
        b = 0
    return "{0:#04x}, {1:#04x}, {2:#04x}, {3:#04x}, {4:#04x}, {5:#04x}, {6:#04x}, {7:#04x}, ".format(*bytes)

glyphs = []
for y in range(im.size[1]//8):
    for x in range(im.size[0]//8):
        b = extract_block(x, y)
        glyphs.append(b)

f = open('tiles.txt', 'w')
for g in glyphs:
    f.write('\t')
    f.write(g)
    f.write('\n')

f.close()
im.close()

f = open('{}.txt'.format(MAP), 'w')
im = Image.open('{}.png'.format(MAP))

pixels = list(im.getdata())

tiles = []
for y in range(im.size[1]//8):
    for x in range(im.size[0]//8):
        b = extract_block(x, y)
        try:
            if b == "0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, ":
                index = 0
            else:
                index = glyphs.index(b) +128
            tiles.append(index)
        except (ValueError):
            tiles.append(0)
            print("Tile not found: {}, {}".format(x, y))

while len(tiles) > 0:
    row = tiles[0:32]
    tiles = tiles[32:]
    line = """
\t{:>3}, {:>3}, {:>3}, {:>3}, {:>3}, {:>3}, {:>3}, {:>3}, {:>3}, {:>3}, {:>3}, {:>3}, {:>3}, {:>3}, {:>3}, {:>3},
\t{:>3}, {:>3}, {:>3}, {:>3}, {:>3}, {:>3}, {:>3}, {:>3}, {:>3}, {:>3}, {:>3}, {:>3}, {:>3}, {:>3}, {:>3}, {:>3},
"""
    f.write(line.format(*row))

f.close()
im.close()

