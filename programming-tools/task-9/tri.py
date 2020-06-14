def read_int(s):
    n = None
    while type(n) != int:
        n = input(s)
        try:
            n = int(n)
        except:
            pass
    return n


def get_triangle_type(a, b, c):
    a, b, c = map(int, sorted((a, b, c)))

    if a + b <= c or b + c <= a or a + c <= b:
        return 'не существует'

    if a == b == c:
        return 'равносторонний'
    elif a == b or b == c:
        return 'равнобедренный'
    elif a**2 + b**2 == c**2:
        return 'прямоугольный'

    return 'разносторонний'


if __name__ == '__main__':
    a = read_int('Введите длину стороны A: ')
    b = read_int('Введите длину стороны B: ')
    c = read_int('Введите длину стороны C: ')

    type_ = get_triangle_type(a, b, c)
    print(f'Треугольник {type_}')
