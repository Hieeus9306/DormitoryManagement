
import os

rooms_file = 'data/Rooms.txt'
invoices_file = 'data/ServiceInvoices.txt'

def read_file_any_encoding(path):
    encodings = ['utf-8', 'utf-16', 'utf-16-le', 'utf-16-be', 'latin-1']
    for enc in encodings:
        try:
            with open(path, 'r', encoding=enc) as f:
                return f.readlines(), enc
        except UnicodeDecodeError:
            continue
    raise Exception(f"Could not decode {path}")

# 1. Find empty rooms
empty_rooms = set()
lines, _ = read_file_any_encoding(rooms_file)
for line in lines:
    line = line.strip()
    if not line:
        continue
    parts = line.split(',')
    if parts[-1].lower() == 'empty':
        empty_rooms.add(parts[0])

print(f"Empty rooms identified: {empty_rooms}")

# 2. Update invoices
new_invoices = []
updated_count = 0
lines, invoice_enc = read_file_any_encoding(invoices_file)
for line in lines:
    line = line.strip()
    if not line:
        continue
    parts = line.split(',')
    if len(parts) < 9:
        new_invoices.append(line)
        continue
        
    room_id = parts[1]
    if room_id in empty_rooms:
        parts[5] = parts[4] # new_elec = old_elec
        parts[7] = parts[6] # new_water = old_water
        parts[8] = "0.0"    # total = 0.0
        updated_count += 1
        
    new_invoices.append(','.join(parts))

# 3. Write back (using detected or default encoding)
with open(invoices_file, 'w', encoding=invoice_enc) as f:
    for line in new_invoices:
        f.write(line + '\n')

print(f"Updated {updated_count} invoices for empty rooms.")
