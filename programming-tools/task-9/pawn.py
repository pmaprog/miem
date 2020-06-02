TEMPLATE = """
   +-+-+-+-+-+-+-+-+
8  |{a8}|{b8}|{c8}|{d8}|{e8}|{f8}|{g8}|{h8}|
   +-+-+-+-+-+-+-+-+
7  |{a7}|{b7}|{c7}|{d7}|{e7}|{f7}|{g7}|{h7}|
   +-+-+-+-+-+-+-+-+
6  |{a6}|{b6}|{c6}|{d6}|{e6}|{f6}|{g6}|{h6}|
   +-+-+-+-+-+-+-+-+
5  |{a5}|{b5}|{c5}|{d5}|{e5}|{f5}|{g5}|{h5}|
   +-+-+-+-+-+-+-+-+
4  |{a4}|{b4}|{c4}|{d4}|{e4}|{f4}|{g4}|{h4}|
   +-+-+-+-+-+-+-+-+
3  |{a3}|{b3}|{c3}|{d3}|{e3}|{f3}|{g3}|{h3}|
   +-+-+-+-+-+-+-+-+
2  |{a2}|{b2}|{c2}|{d2}|{e2}|{f2}|{g2}|{h2}|
   +-+-+-+-+-+-+-+-+
1  |{a1}|{b1}|{c1}|{d1}|{e1}|{f1}|{g1}|{h1}|
   +-+-+-+-+-+-+-+-+
    a b c d e f g h
"""

letters = ('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h')
ciphers = ('1', '2', '3', '4', '5', '6', '7', '8')
figures = (*(f'Пешка #{i}' for i in range(1, 9)), 'Ладья #1', 'Конь #1',
           'Слон #1', 'Ферзь', 'Король', 'Слон #2', 'Конь #2', 'Ладья #2')
coords = {}



def get_coord(figure=None):
   while True:
      if figure:
         print(f'Введите координаты {figure}: ', end='')
      coord = input()
      if len(coord) == 2 and coord[0] in letters and coord[1] in ciphers:
         return coord


def set_positions(player):
   for f in figures:
      if f.startswith('Пешка'): symb = 'p'
      elif f.startswith('Ладья'): symb = 'r'
      elif f.startswith('Конь'): symb = 'n'
      elif f.startswith('Слон'): symb = 'b'
      elif f.startswith('Ферзь'): symb = 'q'
      elif f.startswith('Король'): symb = 'k'
      
      if player == 'b':
         symb = symb.upper()

      coord = None
      while coords[coord] != ' ':
         coord = get_coord(f)
      coords[coord] = symb


def main():
   global coords
   coords = {l+c: ' ' for l in letters for c in ciphers}
   coords[None] = None
   print('Расстановка белых фигур')
   set_positions('w')
   print('Расстановка черных фигур')
   set_positions('b')
   print(TEMPLATE.format(**coords))

   coord_from = None
   while coords[coord_from] != 'p':
      print('Введите координаты белой пешки, которой ходить: ', end='')
      coord_from = get_coord()

   while True:
      print('Введите координаты куда поставить пешку: ', end='')
      coord_to = get_coord()
      l, c = coord_to[0], coord_to[1]

      if int(c) <= 2:
         continue

      allowed_ciphers = [chr(ord(c)-1)]

      symb = coords[coord_to]
      if symb != ' ' and symb == symb.upper():
         if l == 'a':
            allowed_letters = ('b',)
         elif l == 'h':
            allowed_letters = ('g',)
         else:
            allowed_letters = (chr(ord(l)-1), chr(ord(l)+1))
      else:
         allowed_letters = (l,)
         if c == '4':
            allowed_ciphers.append('2')

      if coord_from[0] in allowed_letters and coord_from[1] in allowed_ciphers:
         break

   coords[coord_from] = ' '
   coords[coord_to] = 'p'
   print(TEMPLATE.format(**coords))


if __name__ == '__main__':
   main()
