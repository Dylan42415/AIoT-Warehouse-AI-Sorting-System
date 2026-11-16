import os
import requests
import json
import time
from PIL import Image
import io

OUTPUT_BASE = r"C:\Users\Dfault\Downloads\AIOT_Project\dataset"
USER_AGENT = "AIoTStudentAcademicDownloader/3.0 (edu@ite.edu.sg)"

CHARGER_QUERIES = [
    'Apple USB charger',
    'Baseus USB charger',
    'USB phone charger plug',
    'USB wall charger adapter',
    'Samsung USB charger',
    'USB power adapter plug',
    'Quick Charge USB plug',
    'iPhone power adapter',
    'USB-C power adapter 20W'
]

MOUSE_QUERIES = [
    'computer mouse',
    'optical mouse',
    'wireless computer mouse',
    'USB optical mouse',
    'gaming mouse',
    'Logitech mouse',
    'Razer mouse',
    'ergonomic computer mouse'
]

KEYBOARD_QUERIES = [
    'computer keyboard',
    'mechanical keyboard',
    'USB keyboard',
    'PC keyboard',
    'wireless computer keyboard'
]

BACKGROUND_QUERIES = [
    'conveyor belt texture',
    'black rubber conveyor surface',
    'empty table surface',
    'wood desk texture',
    'laminate desk surface',
    'plain tabletop background',
    'empty workshop table'
]

# Negative filters to reject bad/noisy images
NEGATIVE_FILTERS = {
    'mouse': ['die', 'chip', 'sensor', 'circuit', 'amiga', 'screen', 'monitor', 'laptop', 'rodent', 'animal', 'rat', 'disassembly', 'teardown', 'microchip'],
    'charger': ['laptop', 'notebook', 'car', 'solar', 'station', 'battery pack', 'schematic', 'diagram', 'icon'],
    'keyboard': ['blur', 'abstract', 'glow', 'typing hands', 'piano', 'organ'],
    'background': ['fan', 'ventilator', 'furniture', 'cabinet', 'wardrobe', 'chair', 'bookcase', 'living room', 'person', 'face', 'portrait']
}

def download_category(cat_name, queries, target=100):
    cat_dir = os.path.join(OUTPUT_BASE, cat_name)
    os.makedirs(cat_dir, exist_ok=True)
    
    # 1. Clear old files to ensure fresh 16/11 timestamp
    print(f"\n[16/11 Clean] Purging old files in {cat_name}...")
    for f in os.listdir(cat_dir):
        try:
            os.remove(os.path.join(cat_dir, f))
        except:
            pass
            
    saved = 0
    session = requests.Session()
    session.headers.update({"User-Agent": USER_AGENT})
    seen = set()
    neg_list = NEGATIVE_FILTERS.get(cat_name, [])

    for q in queries:
        if saved >= target:
            break
        print(f"[{cat_name}] Searching Wikimedia: '{q}'...")
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
            r = session.get(api_url, params=params, timeout=12)
            items = r.json().get("query", {}).get("search", [])
        except Exception as e:
            print("  Search error:", e)
            continue

        for it in items:
            if saved >= target:
                break
            title = it.get("title", "")
            if not title or title in seen:
                continue
            seen.add(title)
            
            # Check negative filter
            lower_title = title.lower()
            if any(bad in lower_title for bad in neg_list):
                continue
                
            info_params = {
                "action": "query",
                "format": "json",
                "titles": title,
                "prop": "imageinfo",
                "iiprop": "url|mime"
            }
            try:
                ir = session.get(api_url, params=info_params, timeout=10)
                pages = ir.json().get("query", {}).get("pages", {})
                for _, page in pages.items():
                    info = page.get("imageinfo", [])
                    if not info:
                        continue
                    img_url = info[0].get("url")
                    mime = info[0].get("mime", "")
                    if not img_url or ("jpeg" not in mime and "png" not in mime and "jpg" not in mime):
                        continue
                        
                    raw = session.get(img_url, timeout=15).content
                    im = Image.open(io.BytesIO(raw)).convert("RGB")
                    
                    # Reject images with extreme aspect ratios (banners or tiny icons)
                    w, h = im.size
                    if w < 100 or h < 100 or (w/h > 3.0) or (h/w > 3.0):
                        continue
                        
                    im.thumbnail((400, 400))
                    saved += 1
                    save_path = os.path.join(cat_dir, f"{cat_name}_{saved:03d}.jpg")
                    im.save(save_path, "JPEG", quality=90)
                    if saved % 10 == 0 or saved == target:
                        print(f"  [{cat_name}] Saved: {saved}/{target}")
                    time.sleep(0.12)
                    break
            except Exception:
                continue

    print(f"--> Completed [{cat_name}]: {saved} clean images saved with timestamp 16/11.")
    return saved

def main():
    print("==========================================================")
    print("  AIoT Project: Fresh Clean Dataset Installation (16/11)  ")
    print("==========================================================")
    
    download_category("mouse", MOUSE_QUERIES, target=100)
    download_category("charger", CHARGER_QUERIES, target=100)
    download_category("keyboard", KEYBOARD_QUERIES, target=100)
    download_category("background", BACKGROUND_QUERIES, target=100)
    
    print("\nAll 4 categories refreshed and updated for 16/11/2026!")

if __name__ == "__main__":
    main()
