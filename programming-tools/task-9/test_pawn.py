import pawn
from pawn import get_coord

input_data = []
def set_input_data(data):
    global input_data
    input_data = data
def mock_input():
    return input_data.pop(0)
pawn.input = mock_input


data = open('coords.txt', 'r').readlines()
data = [i[:-1] for i in data]


def test_coord_1():
    l = [chr(l)+str(c) for l in range(ord('a'), ord('i')) for c in range(1, 9)]

    for i in l:
        set_input_data([i])
        assert get_coord() == i

    set_input_data(['k1', 'a1'])
    assert get_coord() == 'a1'


def test_coord_2():
    set_input_data(['[1', 'aa1', '22', 'asdf', 'some123', 'b10', 'b1'])
    assert get_coord() == 'b1'


def test_coord_3():
    set_input_data(['p', '123', '123[a[;', 'h1'])
    assert get_coord() == 'h1'


def test_boundary():
    set_input_data(['a9', 'b0', 'j1', 'a1'])
    assert get_coord() == 'a1'


def test_boundary_pawn():
    from pawn import main
    set_input_data(data + ['a2', 'h3', 'a3'])
    main()
    from pawn import coords
    assert coords['h3'] == ' '
    assert coords['a2'] == ' '
    assert coords['a3'] == 'p'


def test_several_figures():
    from pawn import main
    data2 = data[:-1]
    set_input_data(data2 + ['g8', 'g8', 'h8', 'a2', 'a3'])
    main()


def test_not_pawn():
    from pawn import main
    set_input_data(data + ['a1', 'b1', 'c1', 'a2', 'a3'])
    main()


def test_move_left_or_right():
    from pawn import main
    set_input_data(data + ['e3', 'd3', 'f3', 'e4'])
    main()
    from pawn import coords
    assert coords['f3'] == ' '
    assert coords['d3'] == ' '
    assert coords['e3'] == ' '
    assert coords['e4'] == 'p'


def test_diagonal():
    from pawn import main
    set_input_data(data + ['c2', 'd3', 'b3'])
    main()
    from pawn import coords
    assert coords['d3'] == ' '
    assert coords['b3'] == 'p'


def test_move_back():
    from pawn import main
    set_input_data(data + ['e3', 'e2', 'f1', 'e4'])
    main()
    from pawn import coords
    assert coords['e2'] == ' '
    assert coords['f1'] == 'b'
    assert coords['e3'] == ' '
    assert coords['e4'] == 'p'


def test_more_than_2():
    from pawn import main
    set_input_data(data + ['a2', 'a5', 'a6', 'a3'])
    main()
    from pawn import coords
    assert coords['a5'] == ' '
    assert coords['a6'] == ' '
    assert coords['a2'] == ' '
    assert coords['a3'] == 'p'
