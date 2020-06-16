"""
Скрипт содержит основные функции для работы с базой данных.
"""

def load(path):
  """
  Функция чтения базы данных из файла в оперативную память.
  Параметры: path - путь до файла базы данных
  Возращаемые значения: Если произошла ошибка при чтения файла - False, иначе - dict
  Автор: Рудицкий М.
  """
  try:
    f = open(path, "r", encoding="utf-8")
  
    db = {}
  
    for i in f:
      line = i.split("||")
      line[4] = line[4].strip()
      if len(line) != 5:
        raise Exception()
      insert(db, line[0], (int(line[1]), line[2], line[3], int(line[4])))

    f.close()
  except:
    return False
  
  return db

def save(path, db):
  """
  Функция сохранение базы данных в файл.
  Параметры: path - путь до файла, db - база, которую нужно сохранить
  Возращаемые значения: нет
  Автор: Щупак А.
  """
  try:
    f = open(path, "w", encoding="utf-8")
    for i in db:
      f.write(i + "||" + str(db[i]["year"]) + "||" + db[i]["director"] + "||" + db[i]["genre"] + "||" + str(db[i]["time"]) + "\n")
  finally:
    f.close()

def insert(db, row, values):
  """
  Функция вставки записи в базу.
  Параметры: row - название фильма, values - кортеж, содержаший данные новой записи
  Возращаемые значения: нет
  Автор: Щупак А.
  """

  db[row] = {"year": values[0], "director": values[1], "genre": values[2], "time": values[3]}

def select(db, name, year1, year2, director, genre, time1, time2):
  """
  Функция выбора записей удовлетворяемых условию.
  Параметры: db - бд, name - название фильма, year1 - год от которого искать, year2 - год до которого, director - режиссер, genre - жанр, time1 - продолжительность от которой искать, time2- продолжительность до которой
  Возращаемые значения: result - dict, содержащий результаты выборки
  Автор: Подколзин М.
  """
  result = {}

  if year1 == "":
    year1 = 0
  if year2 == "":
    year2 = 9999
  if time1 == "":
    time1 = 0
  if time2 == "":
    time2 = 9999

  for row in db:
    if name in row:
      if int(db[row]["year"]) >= int(year1) and int(db[row]["year"]) <= int(year2):
        if director in db[row]["director"]:
          f = 1
          for i in genre.split(","):
            if i not in db[row]["genre"]:
              f = 0
          if f and int(db[row]["time"]) >= int(time1) and int(db[row]["time"]) <= int(time2):
            result[row] = db[row]

  return result

def delete(db, row):
  """
  Функция удаления записи.
  Параметры: row - название фильма, который нужно удалить
  Возращаемые значения: нет
  Автор: Рудицкий М.
  """
  del db[row]

def update(db, row, field, value):
  """
  Функция обновления записи.
  Параметры: row - название фильма, который нужно обновить, field - нужно поле, value - новое значение
  Возращаемые значения: нет
  Автор: Рудицкий М.
  """
  if field == "name" and value != row:
    db[value] = db.pop(row)
  else:
    db[row][field] = value