#!/usr/bin/env python3
"""
==============================================================================
 AIoT Final Course Project: Automated Warehouse AI Object Sorting System
 Edge Impulse Automated Dataset Uploader
 Module: AI53003FP | Student: Dylan (Index: 22)
==============================================================================
 Function:
 Reads all sanitized images from `dataset/mouse`, `dataset/charger`, and 
 `dataset/background` and uploads them directly to your Edge Impulse Studio 
 project via the official Ingestion API.
==============================================================================
"""

import os
import sys
import glob
import requests

# -----------------------------------------------------------------------------
# Configuration
# -----------------------------------------------------------------------------
BASE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
DATASET_DIR = os.path.join(BASE_DIR, "dataset")
INGESTION_URL = "https://ingestion.edgeimpulse.com/api/split/files"

# Class Label Definitions (Clean 2-Class Architecture: Target vs Non-Target)
CLASSES = {
    "mouse": os.path.join(DATASET_DIR, "mouse"),
    "background": os.path.join(DATASET_DIR, "background"),
}

def upload_dataset(api_key: str):
    if not api_key or not api_key.startswith("ei_"):
        print("[ERROR] Invalid API Key! Edge Impulse API keys start with 'ei_'")
        print("Find your key at: Edge Impulse Studio -> Dashboard -> Keys")
        return False

    print("=" * 65)
    print("  EDGE IMPULSE AUTOMATED DATASET INGESTION PIPELINE  ")
    print("=" * 65)
    print(f"Dataset root: {DATASET_DIR}")

    total_uploaded = 0
    total_failed = 0

    for label, folder in CLASSES.items():
        if not os.path.exists(folder):
            print(f"[WARN] Folder not found for label '{label}': {folder}")
            continue

        images = glob.glob(os.path.join(folder, "*.jpg")) + glob.glob(os.path.join(folder, "*.png"))
        print(f"\n[CLASS: {label.upper()}] Found {len(images)} images in {folder}")

        for i, img_path in enumerate(images, 1):
            filename = os.path.basename(img_path)
            headers = {
                "x-api-key": api_key,
                "x-label": label,
                "x-disallow-duplicates": "1",
            }

            try:
                with open(img_path, "rb") as f:
                    files = {"data": (filename, f, "image/jpeg")}
                    response = requests.post(INGESTION_URL, headers=headers, files=files, timeout=30)

                if response.status_code == 200:
                    total_uploaded += 1
                    print(f"  [{i}/{len(images)}] OK -> {filename} (Label: {label})")
                else:
                    total_failed += 1
                    print(f"  [{i}/{len(images)}] FAIL ({response.status_code}) -> {filename}: {response.text}")
            except Exception as e:
                total_failed += 1
                print(f"  [{i}/{len(images)}] ERROR -> {filename}: {e}")

    print("\n" + "=" * 65)
    print(f"Ingestion Finished: {total_uploaded} uploaded successfully, {total_failed} failed/duplicates.")
    print("=" * 65)
    return True

if __name__ == "__main__":
    if len(sys.argv) > 1:
        key = sys.argv[1]
    else:
        key = input("Enter your Edge Impulse Project API Key (ei_...): ").strip()

    upload_dataset(key)
