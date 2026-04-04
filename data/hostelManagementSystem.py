from student import Student
from room import Room
from payment import Payment


class HostelManagementSystem:
    def __init__(self):
        self.students = {}
        self.rooms = {}

    def add_room(self, room_number, capacity, room_amount):
        if room_number not in self.rooms:
            self.rooms[room_number] = Room(room_number, capacity, room_amount)
            return f"Room {room_number} added with capacity {capacity} with amount {room_amount}."
        return 'Room already exists.'

    def register_student(self, student_id, name, age, room_number):
        if (
            room_number not in self.rooms or not self.rooms[room_number].is_available()
        ):  # if is_available returns false then it will enter if condition, because "not false" equals true
            return 'Room not available or does not exist.'
        if student_id not in self.students:
            self.rooms[room_number].allocate_room()
            self.students[student_id] = Student(
                student_id, name, age, room_number, 'Pending'
            )
            return f"Student {name} registered and assigned to Room {room_number}."
        return 'Student already registered.'

    def check_in(self, student_id):
        if student_id in self.students:
            student = self.students[student_id]
            if student.payment_status == 'Pending':
                return 'payment pending !'
            return f"Student {student.name} checked in to Room {student.room_number}."
        return 'Student not found.'

    def check_out(self, student_id):
        if student_id in self.students:
            student = self.students.pop(student_id)
            self.rooms[student.room_number].deallocate_room()
            return (
                f"Student {student.name} checked out from Room {student.room_number}."
            )
        return 'Student not found.'

    def record_payment(self, student_id, amount):
        if student_id in self.students:
            student = self.students[student_id]

            if not self.rooms[student.room_number].is_room_amount_correct(amount):
                return f"Payment not correct. Payment should be {self.rooms[student.room_number].room_amount}"
            else:
                Payment.record_payment(student_id, amount)
                student.payment_status = 'Paid'
                return f"Payment of {amount} recorded for Student {student.name}."
        return 'Student not found.'

    def display_students(self):
        return '\n'.join(
            [
                f"ID: {student_id}, Name: {student.name}, Room: {student.room_number}, Payment: {student.payment_status}"
                for student_id, student in self.students.items()
            ]
        )

    def display_rooms(self):
        return '\n'.join(
            [
                f"Room {room.room_number} -> amount:{room.room_amount} rps  --  {room.occupied}/{room.capacity} occupied. "
                for room in self.rooms.values()
            ]
        )
