import random

def electricity_fee(old, new):
    used = new - old
    if used <= 0: return 0
    tiers = [50, 100, 150, 200]
    rates = [2500, 3000, 3600, 4000]
    
    total = 0
    if used <= tiers[0]:
        total = used * rates[0]
    elif used <= tiers[1]:
        total = tiers[0] * rates[0] + (used - tiers[0]) * rates[1]
    elif used <= tiers[2]:
        total = (tiers[0] * rates[0] + 
                 (tiers[1] - tiers[0]) * rates[1] + 
                 (used - tiers[1]) * rates[2])
    else:
        total = (tiers[0] * rates[0] + 
                 (tiers[1] - tiers[0]) * rates[1] + 
                 (tiers[2] - tiers[1]) * rates[2] + 
                 (used - tiers[2]) * rates[3])
    return total

def water_fee(old, new):
    if new < old: return 0
    rate = 10000
    return (new - old) * rate

def generate_invoices():
    rooms = []
    try:
        try:
            with open('data/Rooms.txt', 'r', encoding='utf-8') as f:
                lines = f.readlines()
        except UnicodeDecodeError:
            with open('data/Rooms.txt', 'r', encoding='utf-16') as f:
                lines = f.readlines()
        
        for line in lines:
            parts = line.strip().split(',')
            if parts:
                rooms.append(parts[0])
    except FileNotFoundError:
        return "Error: data/Rooms.txt not found"

    rooms.sort()
    
    # Store the last indices for each room to make it look realistic
    # room_indices[room] = (last_elec, last_water)
    room_indices = {}
    for r in rooms:
        room_indices[r] = (round(random.uniform(100, 500), 1), round(random.uniform(10, 100), 1))
    
    invoices = []
    invoice_counter = 1
    
    # Generate for months 3, 4, 5
    for month in [3, 4, 5]:
        # Sort by room for each month as requested
        for room_name in rooms:
            siid = f"SI{invoice_counter:08d}"
            year = 2026
            
            old_elec, old_water = room_indices[room_name]
            
            new_elec = round(old_elec + random.uniform(20, 250), 1)
            new_water = round(old_water + random.uniform(5, 30), 1)
            
            # Update indices for next month
            room_indices[room_name] = (new_elec, new_water)
            
            total = electricity_fee(old_elec, new_elec) + water_fee(old_water, new_water)
            
            # More true statuses (80% chance)
            status = "true" if random.random() < 0.8 else "false"
            
            invoices.append(f"{siid},{room_name},{month},{year},{old_elec},{new_elec},{old_water},{new_water},{round(total, 1)},{status}")
            invoice_counter += 1
    
    return "\n".join(invoices)

if __name__ == "__main__":
    data = generate_invoices()
    with open('data/ServiceInvoices.txt', 'w', encoding='utf-8') as f:
        f.write(data)
