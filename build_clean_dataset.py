import os
import requests
import json
import time
from PIL import Image
import io

Image.MAX_IMAGE_PIXELS = None  # Prevent DecompressionBomb errors

OUTPUT_BASE = r"C:\Users\Dfault\Downloads\AIOT_Project\dataset"
USER_AGENT = "AIoTStudentAcademicDownloader/2.0 (edu@ite.edu.sg)"

# Queries specifically targeting compact phone charger heads / wall plug blocks (NO laptop chargers)
CHARGER_QUERIES = [
    'Apple USB charger',
    'Baseus USB charger',
    'USB phone charger plug',
    'USB wall charger adapter',
    'Samsung USB charger',
    'USB power adapter plug',
    'Quick Charge USB plug',
    'iPhone power adapter',
    'USB-C power adapter 20W',
    'compact USB charger plug'
]

KEYBOARD_QUERIES = [
    'computer keyboard',
    'mechanical keyboard',
    'QWERTY keyboard',
    'USB keyboard',
    'PC keyboard',
    'wireless keyboard'
]

MOUSE_QUERIES = [
    'computer mouse',
    'optical mouse',
    'wireless mouse',
    'USB mouse',
    'gaming mouse',
    'Logitech mouse'
]

BACKGROUND_QUERIES = [
    'desk surface',
    'wooden table surface',
    'empty desk',
    'tabletop texture',
    'conveyor belt surface'
]

def download_clean_category(category_name, queries, target_count=100, clear_first=False):
    out_dir = os.path.join(OUTPUT_BASE, category_name)
    os.makedirs(out_dir, exist_ok=True)
    
    if clear_first:
        print(f"[{category_name}] Clearing old images to ensure 100% pure phone charger heads...")
        for f in os.listdir(out_dir):
            try:
                os.remove(os.path.join(out_dir, f))
            except:
                pass
        saved = 0
    else:
        existing = [f for f in os.listdir(out_dir) if f.lower().endswith(('.jpg', '.jpeg', '.png'))]
        saved = len(existing)
        
    print(f"[{category_name}] Starting with {saved} images. Target: {target_count}")
    if saved >= target_count:
        print(f"[{category_name}] Already satisfied!")
        return saved

    session = requests.Session()
    session.headers.update({"User-Agent": USER_AGENT})
    seen_titles = set()

    for q in queries:
        if saved >= target_count:
            break
            
        print(f"[{category_name}] Querying: '{q}'...")
        api_url = "https://commons.wikimedia.org/w/api.php"
        params = {
            "action": "query",
            "format": "json",
            "list": "search",
            "srsearch": f"{q} filetype:bitmap",
            "srnamespace": 6,
            "srlimit": 50
        }
        
        try:
            resp = session.get(api_url, params=params, timeout=12)
            items = resp.json().get("query", {}).get("search", [])
        except Exception as e:
            print(f"  Search error: {e}")
            continue

        for it in items:
            if saved >= target_count:
                break
            title = it.get("title", "")
            if not title or title in seen_titles:
                continue
            seen_titles.add(title)
            
            # Explicit filter for charger: Reject any laptop / notebook / car / solar power supply
            lower_t = title.lower()
            if category_name == "charger":
                if any(bad in lower_t for bad in ['laptop', 'notebook', 'car', 'station', 'solar', 'battery pack', 'schematic', 'diagram', 'icon']):
                    continue
                    
            info_params = {
                "action": "query",
                "format": "json",
                "titles": title,
                "prop": "imageinfo",
                "iiprop": "url|mime"
            }
            try:
                info_resp = session.get(api_url, params=info_params, timeout=10)
                pages = info_resp.json().get("query", {}).get("pages", {})
                for _, page in pages.items():
                    info = page.get("imageinfo", [])
                    if not info:
                        continue
                    img_url = info[0].get("url")
                    mime = info[0].get("mime", "")
                    if not img_url or ("jpeg" not in mime and "png" not in mime):
                        continue
                        
                    img_data = session.get(img_url, timeout=15).content
                    im = Image.open(io.BytesIO(img_data)).convert("RGB")
                    im.thumbnail((400, 400))
                    
                    saved += 1
                    save_path = os.path.join(out_dir, f"{category_name}_{saved:03d}.jpg")
                    im.save(save_path, "JPEG", quality=88)
                    if saved % 10 == 0 or saved == target_count:
                        print(f"  [{category_name}] Progress: {saved}/{target_count}")
                    time.sleep(0.15)
                    break
            except Exception:
                continue

    print(f"Finished [{category_name}]: {saved} images saved.\n")
    return saved

def main():
    print("=== Building Pure Clean Dataset (Exclusively Phone Charger Heads) ===")
    # 1. Charger: Clear old laptop chargers and download pure phone charger heads
    download_clean_category("charger", CHARGER_QUERIES, target_count=100, clear_first=True)
    
    # 2. Keyboard: Top up to 100
    download_clean_category("keyboard", KEYBOARD_QUERIES, target_count=100, clear_first=False)
    
    # 3. Mouse: Top up to 100
    download_clean_category("mouse", MOUSE_QUERIES, target_count=100, clear_first=False)
    
    # 4. Background: Download 100 empty surface images
    download_clean_category("background", BACKGROUND_QUERIES, target_count=100, clear_first=False)
    
    print("=== Complete! All categories ready for Edge Impulse! ===")

if __name__ == "__main__":
    main()
