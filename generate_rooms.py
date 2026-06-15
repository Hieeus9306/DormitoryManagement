import random

def generate_rooms():
    floors = [1, 2, 3]
    rooms_per_floor = 12
    capacities = [4] * 4 + [6] * 4 + [8] * 4
    prices = {4: 550, 6: 600, 8: 500}
    
    all_rooms = []
    used_ids = set()
    
    for floor in floors:
        for i in range(1, rooms_per_floor + 1):
            room_name = f"P{floor}{i:02d}"
            capacity = capacities[i-1]
            price = prices[capacity]
            
            # Random number of students
            num_students = random.randint(0, capacity)
            if num_students == 0:
                students_str = "empty"
            else:
                students = []
                while len(students) < num_students:
                    prefix = random.choice(["2022", "2023", "2024", "2025"])
                    suffix = "".join([str(random.randint(0, 9)) for _ in range(5)])
                    student_id = prefix + suffix
                    if student_id not in used_ids:
                        used_ids.add(student_id)
                        students.append(student_id)
                students.sort()
                students_str = "|".join(students)
            
            all_rooms.append(f"{room_name},{capacity},{price},{students_str}")
    
    return "\n".join(all_rooms)

if __name__ == "__main__":
    print(generate_rooms())
