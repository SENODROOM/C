# payment System 

import csv
from datetime import datetime

class Payment:
    def __init__(self, student_id, amount, date):
        self.student_id = student_id
        self.amount = amount
        self.date = date

    @staticmethod
    def record_payment(student_id, amount):
        with open('payments.csv', 'a') as file:
            writer = csv.writer(file)
            writer.writerow(
                [student_id, amount, datetime.now().strftime('%Y-%m-%d %H:%M:%S')]
            )
