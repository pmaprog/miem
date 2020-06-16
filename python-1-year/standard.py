"""
Скрипт содержит стандартные функции разработчика
"""

def validate_entry(entry, type):
  """
  Функция проверяет поле ввода на наличие недопустимых символов.
  Параметры: entry - объект поля ввода, type - тип поля, "int" - можно только числа, "str" - строка (проверяет на пустоту)
  Возращаемые значения: 1 - если всё в порядке, 0 - если нет
  Автор: Подколзин М.
  """
  text = entry.get()
  if type == "int":
    v = 1 if text.isdigit() else 0
  elif type == "str":
    v = 1 if text != "" else 0

  if v:
    entry.configure(bg="white")
  else:
    entry.configure(bg="pink")

  return v