#GRAPHIC UI

import tkinter as tk
from hostelManagementSystem import HostelManagementSystem

system = HostelManagementSystem()

def add_room():
    room_number = room_number_entry.get()
    capacity = int(room_capacity_entry.get())
    room_amount = int(room_amount_entry.get())
    output_label.config(text=system.add_room(room_number, capacity,room_amount))
    

def register_student():
    student_id = student_id_entry.get()
    name = student_name_entry.get()
    age = int(student_age_entry.get())
    room_number = student_room_entry.get()
    output_label.config(
        text=system.register_student(student_id, name, age, room_number)
    )


def check_in():
    student_id = student_id_entry.get()
    output_label.config(text=system.check_in(student_id))


def check_out():
    student_id = student_id_entry.get()
    output_label.config(text=system.check_out(student_id))


def record_payment():
    student_id = student_id_entry.get()
    amount = float(payment_amount_entry.get())
    output_label.config(text=system.record_payment(student_id, amount))


def display_students():
    output_label.config(text=system.display_students())


def display_rooms():
    output_label.config(text=system.display_rooms())


# Tkinter GUI
root = tk.Tk()
root.title('Hostel Management System') #Tiltle 
#Sub-Tiltle Creation 
tk.Label(root, text='Room Management').grid(row=0, column=0, columnspan=2)
#Headings of Room
tk.Label(root, text='Room Number').grid(row=1, column=0) 
room_number_entry = tk.Entry(root)
room_number_entry.grid(row=1, column=1)
tk.Label(root, text='Capacity').grid(row=2, column=0) 
room_capacity_entry = tk.Entry(root)
room_capacity_entry.grid(row=2, column=1)
tk.Label(root, text='Room Amount').grid(row=3, column=0)
room_amount_entry = tk.Entry(root)
room_amount_entry.grid(row=3, column=1)
tk.Button(root, text='Add Room', command=add_room).grid(row=4, column=0, columnspan=2) #Button sys

#Sub-Tiltle Creation 
tk.Label(root, text='Student Management').grid(row=5, column=0, columnspan=2)
#Headings of Students 
tk.Label(root, text='Student ID').grid(row=6, column=0)
student_id_entry = tk.Entry(root)
student_id_entry.grid(row=6, column=1)
tk.Label(root, text='Name').grid(row=7, column=0)
student_name_entry = tk.Entry(root)
student_name_entry.grid(row=7, column=1)
tk.Label(root, text='Age').grid(row=8, column=0)
student_age_entry = tk.Entry(root)
student_age_entry.grid(row=8, column=1)
tk.Label(root, text='Room').grid(row=9, column=0)
student_room_entry = tk.Entry(root)
student_room_entry.grid(row=9, column=1)
tk.Button(root, text='Register Student', command=register_student).grid(
    row=10, column=0, columnspan=2
)

#Sub-Tiltle Creation 
tk.Label(root, text='Payment Management').grid(row=11, column=0, columnspan=2)
#Heading of Room Payment 
tk.Label(root, text='Payment Amount - Rs').grid(row=12, column=0)
payment_amount_entry = tk.Entry(root)
payment_amount_entry.grid(row=12, column=1)
tk.Button(root, text='Record Payment', command=record_payment).grid(
    row=13, column=0, columnspan=2
)

tk.Button(root, text='Check-in', command=check_in).grid(row=14, column=0)
tk.Button(root, text='Check-out', command=check_out).grid(row=14, column=1)

tk.Button(root, text='Display Students', command=display_students).grid(
    row=15, column=0
)
tk.Button(root, text='Display Rooms', command=display_rooms).grid(row=15, column=1)

output_label = tk.Label(root, text='', wraplength=400, justify='left')
output_label.grid(row=16, column=0, columnspan=2)

root.mainloop()
