import random

def generate_contracts():
    student_room_map = []
    
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
                room_id = parts[0]
                students_part = parts[3]
                if students_part != 'empty':
                    ids = students_part.split('|')
                    for sid in ids:
                        student_room_map.append((sid, room_id))
    except FileNotFoundError:
        return "Error: data/Rooms.txt not found"

    # Sort by student ID
    student_room_map.sort()

    contracts = []
    contract_counter = 1
    
    # academic years map: (start_year, end_year)
    # If student starts in 2022, they can have contracts for:
    # 2022-2023, 2023-2024, 2024-2025, 2025-2026
    
    for sid, rid in student_room_map:
        start_year_id = int(sid[:4])
        current_year = 2025
        
        # Generate contracts from their starting year up to 2025-2026
        for year in range(start_year_id, current_year + 1):
            ctid = f"CT{contract_counter:08d}"
            start_date = f"01/09/{year}"
            end_date = f"31/08/{year + 1}"
            
            # Past contracts are likely paid (true), current one might be false
            if year < current_year:
                is_paid = "true" if random.random() < 0.95 else "false"
            else:
                is_paid = random.choice(["true", "false"])
            
            contracts.append(f"{ctid},{sid},{rid},{start_date},{end_date},{is_paid}")
            contract_counter += 1

    return "\n".join(contracts)

if __name__ == "__main__":
    data = generate_contracts()
    with open('data/Contracts.txt', 'w', encoding='utf-8') as f:
        f.write(data)
