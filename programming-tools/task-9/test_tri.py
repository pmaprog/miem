from mock_input import set_keyboard_input, get_display_output
from tri import read_int, get_triangle_type


def test_read_int():
    set_keyboard_input(['ak123', '1,1', '123a', '10'])
    assert read_int('') == 10

def test_negative():
    set_keyboard_input(['-10', '10', '20'])

def test_all_equal():
    set_keyboard_input(['123', '123', '123'])

def test_two_equal():
    set_keyboard_input(['123', '10', '123'])

def test_all_different():
    set_keyboard_input(['1', '2', '3'])

def test_real():
    set_keyboard_input(['1', '2', '3.3', '3'])

def test_boundary():
    assert get_triangle_type(1, 1, 2) == 'не существует'

def test_existence_1():
    assert get_triangle_type(1, 1, 2) == 'не существует'

def test_existence_2():
    assert get_triangle_type(1, 2, 1) == 'не существует'

def test_existence_3():
    assert get_triangle_type(2, 1, 1) == 'не существует'

def test_a_eq_b():
    assert get_triangle_type(3, 3, 2) == 'равнобедренный'

def test_b_eq_c():
    assert get_triangle_type(2, 3, 3) == 'равнобедренный'

def test_a_eq_b_eq_c():
    assert get_triangle_type(3, 3, 3) == 'равносторонний'
    assert get_triangle_type(4, 4, 4) == 'равносторонний'
    assert get_triangle_type(5, 5,  5) == 'равносторонний'

def test_rectangular():
    assert get_triangle_type(3, 4, 5) == 'прямоугольный'
    assert get_triangle_type(20, 21, 29) == 'прямоугольный'

def test_versatile():
    assert get_triangle_type(3, 4, 6) == 'разносторонний'
    assert get_triangle_type(4, 5, 7) == 'разносторонний'

def test_all_0():
    assert get_triangle_type(0, 0, 0) == 'не существует'

def test_all_negative():
    assert get_triangle_type(-2, -3, -4) == 'не существует'
