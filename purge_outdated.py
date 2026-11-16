import os
import shutil

base = r"C:\Users\Dfault\Downloads\AIOT_Project\dataset"
backup_trash = r"C:\Users\Dfault\Downloads\AIOT_Project\purged_noisy_images\outdated_non_resembling"
os.makedirs(backup_trash, exist_ok=True)

# 1. Antique wooden prototypes, trackball consoles, and bare undersides that don't resemble modern mice:
mouse_remove = [
    'mouse_004.jpg', # Antique metal underside
    'mouse_005.jpg', # 1964 wooden block prototype (user uploaded screenshot)
    'mouse_028.jpg', # Stationary desk trackball console with giant pool ball
    'mouse_029.jpg', # 1960s clear acrylic dome contraption
    'mouse_033.jpg', # Underside plate with barcode only
    'mouse_034.jpg', # 1960s wooden box mechanism
    'mouse_039.jpg', # 1968 Telefunken wooden block
    'mouse_040.jpg', # 1970s Xerox rectangular plastic bricks
    'mouse_057.jpg', # Underside plate only
    'mouse_067.jpg', # Underside plate only
    'mouse_098.jpg', # Apple magic mouse battery door undersides
    'mouse_100.jpg'  # Stationary trackball console
]

# 2. Keyboards: Remove abstract blur, keycap close-up gaps, and diagrams that do not resemble a full keyboard:
keyboard_remove = [
    'keyboard_001.jpg', # Abstract speed blur lines
    'keyboard_009.jpg', # Abstract speed blur lines
    'keyboard_020.jpg', # Diagram schematic
    'keyboard_021.jpg', # Diagram schematic
    'keyboard_044.jpg', # Extreme macro gap between 2 keys
    'keyboard_048.jpg', # Store grocery aisle
    'keyboard_049.jpg', # Electronics matrix
    'keyboard_050.jpg'  # Loose blue LED key switches
]

m_purged = 0
for f in mouse_remove:
    src = os.path.join(base, 'mouse', f)
    if os.path.exists(src):
        shutil.move(src, os.path.join(backup_trash, f))
        m_purged += 1

k_purged = 0
for f in keyboard_remove:
    src = os.path.join(base, 'keyboard', f)
    if os.path.exists(src):
        shutil.move(src, os.path.join(backup_trash, f))
        k_purged += 1

m_remain = len(os.listdir(os.path.join(base, "mouse")))
k_remain = len(os.listdir(os.path.join(base, "keyboard")))
c_remain = len(os.listdir(os.path.join(base, "charger")))

print(f"Purged {m_purged} antique/non-resembling mice. Clean modern mice remaining: {m_remain}")
print(f"Purged {k_purged} non-resembling keyboards. Clean keyboards remaining: {k_remain}")
print(f"Clean modern chargers remaining: {c_remain}")
