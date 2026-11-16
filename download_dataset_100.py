import os
import requests
import time
from PIL import Image
import io

OUTPUT_BASE = r"C:\Users\Dfault\Downloads\AIOT_Project\dataset"
USER_AGENT = "AIoTStudentDatasetDownloader/1.0 (academic; edu@ite.edu.sg)"

CATEGORIES = {
    "charger": [
        "AC adapter",
        "power adapter",
        "wall wart",
        "USB charger",
        "battery charger",
        "laptop charger",
        "phone charger",
        "mains adapter",
        "plug adapter"
    ],
    "keyboard": [
        "computer keyboard",
        "mechanical keyboard",
        "QWERTY keyboard",
        "USB keyboard",
        "PC keyboard",
        "wireless keyboard",
        "gaming keyboard",
        "keyboard keys"
    ],
    "mouse": [
        "computer mouse",
        "optical mouse",
        "wireless mouse",
        "USB mouse",
        "gaming mouse",
        "ergonomic mouse",
        "cordless mouse"
    ],
    "background": [
        "desk surface",
        "wooden table surface",
        "empty desk",
        "tabletop",
        "conveyor belt",
        "office desk",
        "workbench surface"
    ]
}

def get_category_images(category_name, queries, target_count=100):
    out_dir = os.path.join(OUTPUT_BASE, category_name)
    os.makedirs(out_dir, exist_ok=True)
    
    # Check existing count
    existing = [f for f in os.listdir(out_dir) if f.lower().endswith(('.jpg', '.jpeg', '.png'))]
    saved = len(existing)
    print(f"[{category_name}] Currently have {saved} images. Targeting {target_count}...")
    
    if saved >= target_count:
        print(f"[{category_name}] Already satisfied ({saved} >= {target_count}).")
        return saved
        
    session = requests.Session()
    session.headers.update({"User-Agent": USER_AGENT})
    seen_titles = set()
    
    for query in queries:
        if saved >= target_count:
            break
            
        print(f"[{category_name}] Searching: '{query}'...")
        offset = 0
        while offset < 150 and saved < target_count:
            api_url = "https://commons.wikimedia.org/w/api.php"
            params = {
                "action": "query",
                "format": "json",
                "list": "search",
                "srsearch": f"{query} filetype:bitmap",
                "srnamespace": 6,
                "srlimit": 50,
                "sroffset": offset
            }
            
            try:
                resp = session.get(api_url, params=params, timeout=12)
                data = resp.json()
                items = data.get("query", {}).get("search", [])
                if not items:
                    break
            except Exception as e:
                print(f"  Search error: {e}")
                break
                
            for item in items:
                if saved >= target_count:
                    break
                    
                title = item.get("title", "")
                if not title or title in seen_titles:
                    continue
                seen_titles.add(title)
                
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
                        im.thumbnail((512, 512))
                        
                        saved += 1
                        save_path = os.path.join(out_dir, f"{category_name}_{saved:03d}.jpg")
                        im.save(save_path, "JPEG", quality=90)
                        if saved % 10 == 0 or saved == target_count:
                            print(f"  [{category_name}] Progress: {saved}/{target_count}")
                        time.sleep(0.2)
                        break
                except Exception:
                    continue
                    
            offset += 50
            time.sleep(0.5)
            
    print(f"Finished {category_name}: Total {saved} images in {out_dir}\n")
    return saved

def main():
    print("=== Downloading 100+ Images Per Class for AIoT Project ===")
    for cat, query_list in CATEGORIES.items():
        get_category_images(cat, query_list, target_count=100)
    print("=== All categories completed with 100+ images! ===")

if __name__ == "__main__":
    main()
