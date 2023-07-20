import serial
import time
import json
import tkinter as tk
from tkinter import ttk
import requests

ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)  # Configura el puerto serie

def read_serial_data():
    data = ''
    while True:
        char = ser.read().decode('utf-8')  # Lee un carácter del puerto serie y lo decodifica
        if char == '\n':
            break  # Finaliza el bucle cuando se encuentra un carácter de nueva línea
        data += char
    return data.strip()

def update_table():
    data = read_serial_data()
    if data:
        try:
            json_data = json.loads(data)  # Intenta cargar los datos como un objeto JSON
            print(json_data)
            table.delete(*table.get_children())  # Limpia la tabla antes de agregar nuevos datos

            # Agrega los datos a la tabla omitiendo la columna "UserId"
            table.insert("", "end", values=(
                json_data["externalTemp"],
                json_data["externalHum"],
                json_data["internalTemp"],
                json_data["phValue"],
                json_data["tsValue"],
                json_data["tensionValue"],
                json_data["tdsValue"],
                json_data["level"]
            ))

            # Envía los datos al servidor mediante una solicitud POST
            url = "https://aqua-qa.sytes.net/api/report"  # Nueva URL de la API
            headers = {'Content-Type': 'application/json'}
            response = requests.post(url, json=json_data, headers=headers)  # Realiza la solicitud POST
            print('Respuesta del servidor:', response.status_code)

        except json.JSONDecodeError as e:
            print('Error al decodificar JSON:', e)

    root.after(5000, update_table)  # Vuelve a llamar a esta función cada 5 segundos

root = tk.Tk()
root.title("aquaqa")
root.configure(bg="#A8E3E7")  # Establece el color de fondo de la ventana

# Establece la fuente Poppins
font_style = ("Poppins", 12)

header_label = tk.Label(root, text="aquaqa", font=("Poppins", 20), bg="#A8E3E7")
header_label.pack(pady=10)

# Crea la tabla
columns = [
    "External Temp",
    "External Hum",
    "Internal Temp",
    "pH Value",
    "TS Value",
    "Tension Value",
    "TDS Value",
    "Level"
]
table = ttk.Treeview(root, columns=columns, show='headings', height=5)
table.pack(padx=20, pady=10)

# Establece el encabezado de la tabla con un color y tamaño diferente
style = ttk.Style()
style.configure("Treeview.Heading", font=("Poppins", 14, "bold"), foreground="#3CC0C9")

# Configura las columnas
for col in columns:
    table.heading(col, text=col, anchor=tk.CENTER)
    table.column(col, anchor=tk.CENTER, width=100)

update_table()  # Inicia el proceso de actualización de la tabla

root.mainloop()
