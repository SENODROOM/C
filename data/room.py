# room System

class Room:
    def __init__(self, room_number, capacity, room_amount):
        self.room_number = room_number
        if self.is_capacity_valid(capacity):
            self.capacity = capacity
        else:
            raise ValueError("Capacity cannot be negative.")
        if self.is_room_amount_valid(room_amount):
            self.room_amount = room_amount
        else:
            raise ValueError("Room amount cannot be negative.")
        self.occupied = 0

    def is_capacity_valid(self, capacity):
        return capacity >= 0

    def is_room_amount_valid(self, room_amount):
        return room_amount >= 0

    def is_available(self):
        return self.occupied < self.capacity

    def is_room_amount_correct(self, amount):
        return self.room_amount == amount

    def allocate_room(self):
        if self.is_available():
            self.occupied += 1
            return True
        return False

    def deallocate_room(self):
        if self.occupied > 0:
            self.occupied -= 1
            return True
        return False

