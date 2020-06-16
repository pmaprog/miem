"""
Основной скрипт программы
"""

import mydb
import shelve as sh

from tkinter import *
from tkinter import ttk
from tkinter import filedialog as fd
from tkinter import messagebox as mb

import sys
sys.path.append("../library/")
import standard as s

class Application:
  """Основной класс программы, здесь содержатся методы для создания и последующего управления программой"""
  def __init__(self, root):
    """
    Конструктор создает окно и отображает его на экран.
    Параметры: root - объект окна Tkinter
    Возращаемые значения: нет
    Автор: Щупак А.
    """
    self.root = root
    self.root.title("База данных фильмов v1.0")
    self.root.geometry("1024x768")
    self.root.protocol("WM_DELETE_WINDOW", self.on_exit)

    self.db = {}
    self.results = {}

    self.is_search = 0

    self.load_config()
    self.create_menus()
    self.create_widgets()

  def load_config(self):
    """
    Функция для загрузки настроек программы.
    Параметры: нет
    Возращаемые значения: нет
    Автор: Щупак А.
    """
    self.config = sh.open("config.cfg")
    if "table-h-ff" not in self.config:
      self.config["table-h-ff"] = "Tahoma"
      self.config["table-h-fz"] = 12
      self.config["table-h-bg"] = "#cccccc"
      self.config["table-ff"] = "Tahoma"
      self.config["table-fz"] = 10

  def create_menus(self):
    """
    Функция для создания меню, используемых в программе.
    Параметры: нет
    Возращаемые значения: нет
    Автор: Подколзин М.
    """
    self.menu = Menu(self.root)

    self.menu.add_command(label="Загрузить", command=self.load)
    self.menu.add_command(label="Сохранить", command=self.save)
    self.menu.add_command(label="Добавить запись", command=lambda: DialogEditing(self, 0))
    self.menu.add_command(label="Поиск", command=lambda: DialogSearching(self))
    self.menu.add_command(label="Настройки", command=lambda: DialogConfig(self))
    self.menu.add_command(label="Выход", command=self.on_exit)

    self.table_rc_menu = Menu(tearoff=0)
    self.table_rc_menu.add_command(label="Изменить", command=lambda: DialogEditing(self, 1))
    self.table_rc_menu.add_command(label="Удалить", command=self.on_deleting)

    self.root.config(menu=self.menu)

  def create_widgets(self):
    """
    Функция создает виджеты главного окна программы.
    Параметры: нет
    Возращаемые значения: нет
    Автор: Подколзин М.
    """
    frm_table = Frame(self.root)
    frm_table.pack(fill=BOTH, expand=1)
    self.table = ttk.Treeview(frm_table, columns=("year", "director", "genre", "time"), selectmode=BROWSE)
    ysb = ttk.Scrollbar(frm_table, orient="vertical", command=self.table.yview)
    ysb.pack(side=RIGHT, fill=Y)
    self.table.configure(yscroll=ysb.set)
    self.table.pack(fill=BOTH, expand=1)
    self.table.heading('#0', text="Название", anchor=W)
    self.table.heading("year", text="Год", anchor=W)
    self.table.heading("director", text="Режиссер", anchor=W)
    self.table.heading("genre", text="Жанр", anchor=W)
    self.table.heading("time", text="Продолжительность", anchor=W)
    self.table.bind("<Button-3>", self.on_table_right_click)
    self.update_config()

  def update_config(self):
    """
    Функция обновляет настройки (внешний вид) программы.
    Параметры: нет
    Возращаемые значения: нет
    Автор: Рудицкий М.
    """
    ttk.Style().configure("Treeview.Heading", font=(self.config["table-h-ff"], self.config["table-h-fz"]), background=self.config["table-h-bg"])
    ttk.Style().configure("Treeview", font=(self.config["table-ff"], self.config["table-fz"]))

  def print_records(self, data):
    """
    Функция выводит в таблицу данные.
    Параметры: data - dict (бд)
    Возращаемые значения: нет
    Автор: Подколзин М.
    """
    for i in self.table.get_children():
      self.table.delete(i)

    if self.is_search:
      ttk.Style().configure("Treeview", background="yellow")
    else:
      ttk.Style().configure("Treeview", background="white")

    for row in data:
      self.table.insert('', END, text=row, values=(data[row]["year"], data[row]["director"], data[row]["genre"], data[row]["time"]))

  def load(self):
    """
    Функция выводит на экран модальное окно открытия файла и записывает обработанные данные файла в переменную базы данных.
    Параметры: нет
    Возращаемые значения: нет
    Автор: Рудицкий М.
    """
    dlg = fd.Open(self.root, filetypes = [('Файл базы данных', '*.txt')])
    path = dlg.show()
    if path:
      self.db = mydb.load(path)
      if self.db is False:
        mb.showerror("Ошибка", "Произошла ошибка при чтении файла!")
      else:
        self.print_records(self.db)

  def save(self):
    """
    Функция выводит на экран модальное окно сохранения файла и записывает данные базы данных в выбранный файл.
    Параметры: нет
    Возращаемые значения: нет
    Автор: Щупак А.
    """
    path = fd.asksaveasfilename(defaultextension=".txt")
    if path:
      if self.is_search:
        mydb.save(path, self.results)
      else:
        mydb.save(path, self.db)
      mb.showinfo("Сохранение", "Файл сохранен!")

  def on_deleting(self):
    """
    Функция обработчик события нажатия на кнопку "Удалить". Производит удаление выбранной записи.
    Параметры: нет
    Возращаемые значения: нет
    Автор: Рудицкий М.
    """
    row = self.table.item(self.table.selection())
    if row["values"]:
      if mb.askyesno(title="Удалить запись", message="Удалить запись\n\"" + row["text"] + "\""):
        mydb.delete(self.db, row["text"])
        self.print_records(self.db)

  def on_table_right_click(self, event):
    """
    Функция обработчик события нажатия на правую кнопку мыши в таблице. Выводит контексное меню.
    Параметры: event - объект события
    Возращаемые значения: нет
    Автор: Подколзин М.
    """
    region = self.table.identify("region", event.x, event.y)
    if self.table.selection() and (region == "cell" or region == "tree"):
      self.table_rc_menu.unpost()
      self.table_rc_menu.post(event.x_root, event.y_root)

  def on_exit(self):
    """
    Функция обработчик события закрытия программы. Выводит предупреждающее сообщение и, если нужно, выходит из программы.
    Параметры: нет
    Возращаемые значения: нет
    Автор: Рудицкий М.
    """
    if mb.askyesno(title="Выход", message="Вы уверены?\nНе сохраненные данные будут утеряны!"):
      self.config.close()
      self.root.destroy()

class DialogEditing:
  """Класс диалога изменения/добавления записи"""
  def __init__(self, app, is_edit):
    """
    Конструктор создает модальное окно и отображает его на экран.
    Параметры: app - объект Application, is_edit - если 1, то нажата кнопка изменения записи, 0 - добавления
    Возращаемые значения: нет
    Автор: Щупак А.
    """
    self.app = app
    self.is_edit = is_edit
    self.row = app.table.item(app.table.selection())

    if not self.row["values"] and is_edit:
      return

    self.dlg = Toplevel(app.root)
    self.dlg.resizable(False, False)
    self.dlg.grab_set()
    self.dlg.focus_force()
    self.dlg.title("Изменить запись" if is_edit else "Добавить запись")

    self.create_widgets()

  def create_widgets(self):
    """
    Функция создает виджеты модального окна.
    Параметры: нет
    Возращаемые значения: нет
    Автор: Подколзин М.
    """
    frame = Frame(self.dlg)
    frame.grid(padx=10, pady=10)

    Label(frame, text="Название: ").grid(row=0, column=0, sticky=W)
    self.name = Entry(frame)
    if self.is_edit:
      self.name.insert(0, self.row["text"])
    self.name.grid(row=0, column=1)

    Label(frame, text="Год: ").grid(row=1, column=0, sticky=W)
    self.year = Entry(frame)
    if self.is_edit:
      self.year.insert(0, self.row["values"][0])
    self.year.grid(row=1, column=1)

    Label(frame, text="Режиссер: ").grid(row=2, column=0, sticky=W)
    self.director = Entry(frame)
    if self.is_edit:
      self.director.insert(0, self.row["values"][1])
    self.director.grid(row=2, column=1)

    Label(frame, text="Жанр: ").grid(row=3, column=0, sticky=W)
    self.genre = Entry(frame)
    if self.is_edit:
      self.genre.insert(0, self.row["values"][2])
    self.genre.grid(row=3, column=1)

    Label(frame, text="Время: ").grid(row=4, column=0, sticky=W)
    self.time = Entry(frame)
    if self.is_edit:
      self.time.insert(0, self.row["values"][3])
    self.time.grid(row=4, column=1)

    Button(frame, text="Принять", command=self.on_accept).grid(row=5, column=0, sticky=W)
    Button(frame, text="Отмена", command=lambda: self.dlg.destroy()).grid(row=5, column=1, sticky=E)

  def on_accept(self):
    """
    Функция обработчик события нажатия на кнопку "Принять". Добавляет/изменяет запись.
    Параметры: нет
    Возращаемые значения: нет
    Автор: Подколзин М.
    """
    if s.validate_entry(self.name, "str") + s.validate_entry(self.year, "int") + s.validate_entry(self.director, "str") + s.validate_entry(self.genre, "str") + s.validate_entry(self.time, "int") == 5:
      if self.name.get() in self.app.db and self.name.get() != self.row["text"]:
        mb.showerror("Ошибка", "Запись уже есть в базе!")
      else:
        if self.is_edit:
          mydb.update(self.app.db, self.row["text"], "year", self.year.get())
          mydb.update(self.app.db, self.row["text"], "director", self.director.get())
          mydb.update(self.app.db, self.row["text"], "genre", self.genre.get())
          mydb.update(self.app.db, self.row["text"], "time", self.time.get())
          mydb.update(self.app.db, self.row["text"], "name", self.name.get())
        else:
          mydb.insert(self.app.db, self.name.get(), (self.year.get(), self.director.get(), self.genre.get(), self.time.get()))

        self.app.print_records(self.app.db)
        self.dlg.destroy()
    else:
      mb.showerror("Ошибка", "Заполните поля корректно!")

class DialogSearching:
  """Класс модального окна "Поиск"""
  def __init__(self, app):
    """
    Конструктор создает модальное окно и отображает его на экран.
    Параметры: app - объект Application
    Возращаемые значения: нет
    Автор: Щупак А.
    """
    self.app = app

    app.menu.entryconfig("Загрузить", state="disabled")
    app.menu.entryconfig("Поиск", state="disabled")

    self.dlg = Toplevel(app.root)
    self.dlg.title("Поиск")
    self.dlg.resizable(False, False)
    self.dlg.focus_force()
    self.dlg.protocol("WM_DELETE_WINDOW", self.on_close)

    self.create_widgets()

  def create_widgets(self):
    """
    Функция создает виджеты модального окна.
    Параметры: нет
    Возращаемые значения: нет
    Автор: Рудицкий М.
    """
    frame = Frame(self.dlg)
    frame.pack(padx=10, pady=10, expand=1)
    frm_panel = Frame(frame)
    frm_panel.pack()

    Label(frm_panel, text="Название: ").grid(row=0, column=0, sticky=W)
    self.name = Entry(frm_panel)
    self.name.grid(row=0, column=1)

    Label(frm_panel, text="Год: ").grid(row=1, column=0, sticky=W)
    self.year1 = Entry(frm_panel)
    self.year1.grid(row=1, column=1)
    Label(frm_panel, text=" - ").grid(row=1, column=2, sticky=W)
    self.year2 = Entry(frm_panel)
    self.year2.grid(row=1, column=3)

    Label(frm_panel, text="Режиссер: ").grid(row=2, column=0, sticky=W)
    self.director = Entry(frm_panel)
    self.director.grid(row=2, column=1)

    Label(frm_panel, text="Жанр: ").grid(row=3, column=0, sticky=W)
    self.genre = Entry(frm_panel)
    self.genre.grid(row=3, column=1)

    Label(frm_panel, text="Время: ").grid(row=4, column=0, sticky=W)
    self.time1 = Entry(frm_panel)
    self.time1.grid(row=4, column=1)
    Label(frm_panel, text=" - ").grid(row=4, column=2, sticky=W)
    self.time2 = Entry(frm_panel)
    self.time2.grid(row=4, column=3)

    Label(frm_panel).grid(row=5)

    Button(frm_panel, text="Найти", command=self.on_find).grid(row=6, column=0, sticky=W)
    Button(frm_panel, text="Закрыть", command=self.on_close).grid(row=6, column=1, sticky=W)

  def on_close(self):
    """
    Функция обработчик события закрытия программы. Возращает таблицу в исходный вид.
    Параметры: нет
    Возращаемые значения: нет
    Автор: Рудицкий М.
    """
    self.app.menu.entryconfig("Загрузить", state="normal")
    self.app.menu.entryconfig("Поиск", state="normal")
    self.app.is_search = 0
    self.app.print_records(self.app.db)
    self.dlg.destroy()

  def validate(self, entry):
    """
    Функция проверки полей ввода.
    Параметры: entry - объект поля ввода
    Возращаемые значения: нет
    Автор: Щупак А.
    """
    if entry.get() != "" and not entry.get().isdigit():
      entry.configure(bg="pink")
      return 0
    else:
      entry.configure(bg="white")
      return 1

  def on_find(self):
    """
    Функция обработчик события нажатия на кнопку "Найти". Отображает в таблице найденые данные.
    Параметры: entry - объект поля ввода
    Возращаемые значения: нет
    Автор: Подколзин М.
    """
    if self.validate(self.time1) + self.validate(self.time2) + self.validate(self.year1) + self.validate(self.year2) == 4:
      self.app.is_search = 1
      self.app.print_records(mydb.select(self.app.db, self.name.get(), self.year1.get(), self.year2.get(), self.director.get(), self.genre.get(), self.time1.get(), self.time2.get()))
    else:
      mb.showerror("Ошибка", "Заполните поля формы корректно!")

class DialogConfig:
  """Класс модальное окна "Настройки" """
  def __init__(self, app):
    """
    Конструктор создает и отображает модальное окно.
    Параметры: app - объект Application
    Возращаемые значения: нет
    Автор: Подколзин М.
    """
    self.app = app

    self.dlg = Toplevel(app.root)
    self.dlg.title("Настройки")
    self.dlg.resizable(False, False)
    self.dlg.grab_set()
    self.dlg.focus_force()

    self.create_widgets()

  def create_widgets(self):
    """
    Функция создает виджеты модального окна.
    Параметры: нет
    Возращаемые значения: нет
    Автор: Подколзин М.
    """
    frame = Frame(self.dlg)
    frame.pack(padx=10, pady=10, expand=1)

    Label(frame, text="Шрифт шапки: ").grid(row=0, column=0, sticky=W)
    self.list1 = StringVar(frame)
    self.list1.set(self.app.config["table-h-ff"])
    self.table_h_ff = OptionMenu(frame, self.list1, "Tahoma", "Comic Sans MS", "Times New Roman").grid(row=0, column=1, sticky=W)

    Label(frame, text="Размер шрифта шапки: ").grid(row=1, column=0, sticky=W)
    self.table_h_fz = Entry(frame)
    self.table_h_fz.insert(END, self.app.config["table-h-fz"])
    self.table_h_fz.grid(row=1, column=1, sticky=W)

    Label(frame, text="Фоновый цвет шапки: ").grid(row=2, column=0, sticky=W)
    self.table_h_bg = Entry(frame)
    self.table_h_bg.insert(END, self.app.config["table-h-bg"])
    self.table_h_bg.grid(row=2, column=1, sticky=W)

    Label(frame, text="Шрифт ячеек: ").grid(row=3, column=0, sticky=W)
    self.list2 = StringVar(frame)
    self.list2.set(self.app.config["table-ff"])
    table_ff = OptionMenu(frame, self.list2, "Tahoma", "Comic Sans MS", "Times New Roman").grid(row=3, column=1, sticky=W)

    Label(frame, text="Размер шрифта ячеек: ").grid(row=4, column=0, sticky=W)
    self.table_fz = Entry(frame)
    self.table_fz.insert(END, self.app.config["table-fz"])
    self.table_fz.grid(row=4, column=1, sticky=W)

    Button(frame, text="Применить", command=self.on_accept).grid(row=5, column=0)
    Button(frame, text="Отмена", command=lambda: self.dlg.destroy()).grid(row=5, column=1)

  def on_accept(self):
    """
    Функция обработчик события нажатия на кнопку "Применить". Изменяет настройки программы.
    Параметры: нет
    Возращаемые значения: нет
    Автор: Подколзин М.
    """
    if s.validate_entry(self.table_h_bg, "str") + s.validate_entry(self.table_fz, "int") + s.validate_entry(self.table_h_fz, "int") == 3:
      self.app.config["table-h-ff"] = self.list1.get()
      self.app.config["table-h-fz"] = self.table_h_fz.get()
      self.app.config["table-h-bg"] = self.table_h_bg.get()
      self.app.config["table-ff"] = self.list2.get()
      self.app.config["table-fz"] = self.table_fz.get()
      self.app.update_config()
      self.dlg.destroy()
    else:
      mb.showerror("Ошибка", "Заполните поля формы корректно!")

root = Tk()
app = Application(root)
root.mainloop()