import random

def generate_students():
    # Read IDs from Rooms.txt
    student_ids = []
    try:
        try:
            with open('data/Rooms.txt', 'r', encoding='utf-8') as f:
                lines = f.readlines()
        except UnicodeDecodeError:
            with open('data/Rooms.txt', 'r', encoding='utf-16') as f:
                lines = f.readlines()
        
        for line in lines:
            parts = line.strip().split(',')
            if len(parts) >= 4:
                students_part = parts[3]
                if students_part != 'empty':
                    ids = students_part.split('|')
                    student_ids.extend(ids)
    except FileNotFoundError:
        return "Error: data/Rooms.txt not found"

    # Remove duplicates and sort
    student_ids = sorted(list(set(student_ids)))

    # Expanded list of English names
    first_names = [
        "James", "Mary", "Robert", "Patricia", "John", "Jennifer", "Michael", "Linda", "David", "Elizabeth",
        "William", "Barbara", "Richard", "Susan", "Joseph", "Jessica", "Thomas", "Sarah", "Christopher", "Karen",
        "Charles", "Lisa", "Daniel", "Nancy", "Matthew", "Betty", "Anthony", "Sandra", "Mark", "Margaret",
        "Emma", "Liam", "Olivia", "Noah", "Ava", "Oliver", "Sophia", "Elijah", "Isabella", "Lucas",
        "Mia", "Mason", "Charlotte", "Ethan", "Amelia", "Logan", "Harper", "James", "Evelyn", "Alexander"
    ]
    last_names = [
        "Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia", "Miller", "Davis", "Rodriguez", "Martinez",
        "Hernandez", "Lopez", "Gonzalez", "Wilson", "Anderson", "Thomas", "Taylor", "Moore", "Jackson", "Martin",
        "Lee", "Perez", "Thompson", "White", "Harris", "Sanchez", "Clark", "Ramirez", "Lewis", "Robinson",
        "Walker", "Young", "Allen", "King", "Wright", "Scott", "Torres", "Nguyen", "Hill", "Flores"
    ]
    
    majors = ["MI1", "MI2", "HF1", "MS2", "MS1"]

    students_data = []
    for sid in student_ids:
        first = random.choice(first_names)
        last = random.choice(last_names)
        full_name = f"{first} {last}"
        
        major = random.choice(majors)
        status = random.choice(["true", "false"])
        
        # Phone: 0 + 9 random digits
        phone = "0" + "".join([str(random.randint(0, 9)) for _ in range(9)])
        
        # Email: first name (lowercase) + sid + @gmail.huts.com
        email_prefix = first.lower()
        email = f"{email_prefix}{sid}@gmail.huts.com"
        
        students_data.append(f"{sid},{full_name},{major},{status},{phone},{email}")

    return "\n".join(students_data)

if __name__ == "__main__":
    data = generate_students()
    with open('data/Students.txt', 'w', encoding='utf-8') as f:
        f.write(data)
