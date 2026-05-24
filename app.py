import os, sys
import tkinter as tk
import tkinter.ttk as ttk
from tkinter import messagebox, filedialog
import threading
from PIL import Image, ImageTk, ImageFile
import uareu4500

ImageFile.LOAD_TRUNCATED_IMAGES = True

def get_asset_path(relative_path):
    """Get absolute path to resource, works for development and PyInstaller"""
    if getattr(sys, '_MEIPASS', False):
        return os.path.join(sys._MEIPASS, relative_path)
    return os.path.join(os.path.abspath("."), relative_path)

icon_path = get_asset_path("asset/favicon.ico")
logo_path = get_asset_path("asset/logo.png")

class FingerPrintStyle:
    def __init__(self, root):
        self.style = ttk.Style(root)
        self.style.theme_use("clam")
        self.style.configure("title.TLabel", font=("Arial", 18, "bold"), background="#f0f0f0", foreground="#0754c6")
        self.style.configure("text.TLabel", font=("Arial", 12), background="#f0f0f0")
        self.style.configure("button.TButton", background="#f0f0f0", foreground="#0754c6", underline="center", font=("Arial", 12))
        self.style.map("button.TButton", 
                       background=[('disabled', '#d3d3d3'), ('active', '#0754c6'), ('!disabled', '#f0f0f0')],
                       foreground=[('disabled', '#a0a0a0'), ('active', '#f0f0f0'), ('!disabled', '#0754c6')])
        self.style.configure("status_black.TLabel", foreground="black")
        self.style.configure("status_blue.TLabel", foreground="blue")
        self.style.configure("status_green.TLabel", foreground="green")
        self.style.configure("status_red.TLabel", foreground="red")

class FingerprintApp:
    def __init__(self, root:tk.Tk):
        self.root = root
        self.root.title("Fingerprint Scanner")
        self.root.iconbitmap(icon_path)
        
        self.width = 800
        self.height = 600
        self.root.geometry(f"{self.width}x{self.height}")
        
        root.columnconfigure(0, weight=1)
        root.columnconfigure(1, weight=15)
        root.columnconfigure(2, weight=1)
        self.file_path = tk.StringVar()
        self.is_scanning = False
        
        self.logo = Image.open(logo_path)
        self.logo = self.logo.resize((100, 100))
        self.logo = ImageTk.PhotoImage(self.logo)

        self.label = ttk.Label(root, text="Dermatalent Indonesia", style="title.TLabel", anchor="center")
        self.label.grid(row=0, column=0, columnspan=3, sticky="ew", padx=10, pady=5)
        
        self.label = ttk.Label(root, text="Dermatalent Logo", style="title.TLabel", anchor="center", image=self.logo)
        self.label.grid(row=0, column=0, sticky="ew", padx=10, pady=5)

        self.select_path_label = ttk.Label(root, text="Save Location", style="text.TLabel", anchor="center")
        self.select_path_label.grid(row=1, column=0, padx=0, pady=5)
        
        self.select_path_value = ttk.Entry(root, textvariable=self.file_path, state="readonly")
        self.select_path_value.grid(row=1, column=1, pady=5, sticky="ew", ipady=9)
        
        self.select_path_button = ttk.Button(root, text="📁 Browse", command=self.select_save_location, width=10, style="button.TButton", compound="center")
        self.select_path_button.grid(row=1, column=2, padx=5, pady=5, ipady=5)
        
        self.canvas = tk.Canvas(root, width=273, height=344, bg="#ffffff")
        self.canvas.grid(row=2, column=0, columnspan=3, pady=10)
        
        self.scan_button = ttk.Button(root, text="Scan Fingerprint", command=self.scan_fingerprint, state=tk.DISABLED, style="button.TButton", compound="center")
        self.scan_button.grid(row=3, column=0, columnspan=3, pady=5)

    def select_save_location(self):
        """Allow the user to choose a save location before scanning."""
        file_path = filedialog.asksaveasfilename(defaultextension=".png", 
                                                 filetypes=[("PNG files", "*.png"),
                                                            ("JPEG files", "*.jpg;*.jpeg"),
                                                            ("All Files", "*.*")])
        if file_path:
            self.file_path.set(file_path)
            self.scan_button.config(state=tk.NORMAL)

    def scan_fingerprint(self):
        """Capture the fingerprint and save it to the preselected location."""
        if self.is_scanning:
            # Cancel the ongoing scan
            uareu4500.cancel_scan()
            return

        if not self.file_path.get():
            messagebox.showerror("Error", "Please select a save location first!")
            return
            
        self.is_scanning = True
        self.scan_button.config(text="Cancel Scanning")
        self.select_path_button.config(state=tk.DISABLED)

        def run_scan():
            save_directory = os.path.dirname(self.file_path.get())
            self.bmp_path_temp = os.path.join(save_directory, "fingerprint_temp.bmp")
            bmp_path = uareu4500.capture_fingerprint(self.bmp_path_temp)
            self.root.after(0, self.on_scan_complete, bmp_path)

        threading.Thread(target=run_scan, daemon=True).start()

    def on_scan_complete(self, bmp_path):
        self.is_scanning = False
        self.scan_button.config(text="Scan Fingerprint")
        self.select_path_button.config(state=tk.NORMAL)

        if bmp_path:
            self.bmp_path = bmp_path
            try:
                image = Image.open(self.bmp_path)
                image.load()
                
                self.tk_image = ImageTk.PhotoImage(image)
                self.canvas.create_image(0, 0, anchor="nw", image=self.tk_image)

                ext = self.file_path.get().split(".")[-1].upper()
                if ext not in ["PNG", "JPG", "JPEG"]:
                    ext = "PNG"
                uareu4500.convert_image(self.bmp_path, self.file_path.get(), format=ext)
                messagebox.showinfo("Saved", f"Fingerprint saved as {self.file_path.get()}")
            except Exception as e:
                messagebox.showerror("Image Error", f"Failed to load image: {e}")
        else:
            messagebox.showwarning("Capture Stopped", "Fingerprint capture was canceled or failed.")

if __name__ == "__main__":
    root = tk.Tk()
    app = FingerprintApp(root)
    FingerPrintStyle(root)
    root.mainloop()
