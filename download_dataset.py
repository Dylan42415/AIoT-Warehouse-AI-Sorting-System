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
        "battery charger"
    ],
    "keyboard": [
        "computer keyboard",
        "mechanical keyboard",
        "QWERTY keyboard",
        "USB keyboard"
    ],
    "mouse": [
        "computer mouse",
        "optical mouse",
        "wireless mouse",
        "USB mouse"
    ]
}

def get_category_images(category_name, queries, target_count=35):
    out_dir = os.path.join(OUTPUT_BASE, category_name)
    os.makedirs(out_dir, exist_ok=True)
    
    session = requests.Session()
    session.headers.update({"User-Agent": USER_AGENT})
    
    saved = 0
    seen_titles = set()
    
    for query in queries:
        if saved >= target_count:
            break
            
        print(f"[{category_name}] Searching: '{query}'...")
        api_url = "https://commons.wikimedia.org/w/api.php"
        params = {
            "action": "query",
            "format": "json",
            "list": "search",
            "srsearch": f"{query} filetype:bitmap",
            "srnamespace": 6,
            "srlimit": 30
        }
        
        try:
            resp = session.get(api_url, params=params, timeout=10)
            items = resp.json().get("query", {}).get("search", [])
        except Exception as e:
            print(f"  Search error: {e}")
            continue
            
        for item in items:
            if saved >= target_count:
                break
                
            title = item.get("title", "")
            if not title or title in seen_titles:
                continue
            seen_titles.add(title)
            
            # Fetch direct URL
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
                    
                    # Download image bytes
                    img_data = session.get(img_url, timeout=15).content
                    
                    # Verify with PIL and resize to max 512x512 to keep uploads fast
                    im = Image.open(io.BytesIO(img_data)).convert("RGB")
                    im.thumbnail((512, 512))
                    
                    saved += 1
                    save_path = os.path.join(out_dir, f"{category_name}_{saved:03d}.jpg")
                    im.save(save_path, "JPEG", quality=90)
                    print(f"  Saved ({saved}/{target_count}): {os.path.basename(save_path)}")
                    time.sleep(0.3)
                    break
            except Exception as ex:
                continue
                
    print(f"Finished {category_name}: {saved} images saved in {out_dir}\n")
    return saved

def main():
    print("=== Scraping Dataset for AIoT Edge Impulse ===")
    for cat, query_list in CATEGORIES.items():
        get_category_images(cat, query_list, target_count=35)
    print("=== All categories downloaded successfully! ===")

if __name__ == "__main__":
    main()
