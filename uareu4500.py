import ctypes
import os
from PIL import Image
from tkinter import filedialog

# Load DLL
dll_path = os.path.abspath("uareu4500.dll")
fingerprint_lib = ctypes.CDLL(dll_path, winmode=0)

# Define function prototype
fingerprint_lib.scan_fingerprint.argtypes = [ctypes.c_char_p]
fingerprint_lib.scan_fingerprint.restype = ctypes.c_int

def capture_fingerprint(output_filename="fingerprint.bmp"):
    """Capture a fingerprint and save as a BMP image."""
    result = fingerprint_lib.scan_fingerprint(output_filename.encode('utf-8'))
    if result == 0:
        print(f"✅ Fingerprint saved as {output_filename}")
        return output_filename
    else:
        print(f"❌ Fingerprint scan failed! Error code: {result}")
        return None

def convert_image(input_filename, output_filename, format="PNG"):
    """Convert BMP fingerprint image to PNG/JPG/JPEG."""
    try:
        format = "JPEG" if format.upper() in ["JPG", "JPEG"] else format.upper()
        image = Image.open(input_filename)
        if image.mode != "RGB":
            image = image.convert("RGB")
        image.save(output_filename, format=format)
        print(f"✅ Converted and saved as {output_filename}")
        return output_filename
    except Exception as e:
        print(f"❌ Failed to convert image: {e}")
        return None

def save_fingerprint():
    """Capture fingerprint and allow user to save as PNG/JPG/JPEG."""
    bmp_path = capture_fingerprint()
    if bmp_path:
        file_path = filedialog.asksaveasfilename(defaultextension=".png", 
                                                 filetypes=[("PNG files", "*.png"),
                                                            ("JPEG files", "*.jpg;*.jpeg"),
                                                            ("All Files", "*.*")])
        if file_path:
            ext = file_path.split(".")[-1].upper()
            if ext not in ["PNG", "JPG", "JPEG"]:
                ext = "PNG"
            convert_image(bmp_path, file_path, format=ext)

if __name__ == "__main__":
    save_fingerprint()