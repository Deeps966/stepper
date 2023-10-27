import pandas as pd
import json
import serial

# File path to your Excel file
excel_file_path = "sample.csv"
ESP_SERIAL_PORT = "/dev/ttyUSB1"
ESP_BAUD_RATE = 115200

# Function to read Excel file and store data in a dictionary
def read_excel_to_dict(file_path):
    try:
        file_extension = file_path.split('.')[-1].lower()
        
        # Read Excel file using pandas
        if file_extension == "xls" or file_extension == "xlsx":
          xls = pd.ExcelFile(file_path)
          df = xls.parse(0)
        elif file_extension == "csv":
          df = pd.read_csv(file_path)
        else:
          raise Exception("Invalid file type: "+file_extension)
        
        # Create an empty list to store data as dictionaries
        data_list = []

        # Iterate through the rows of the DataFrame and store data in the list
        for index, row in df.iterrows():
            data_dict = {
              "row": index,
              "motor1": float(row[0]),
              "motor2": float(row[1]),
              "motor3": float(row[2])
            }
            data_list.append(data_dict)

        return data_list

    except Exception as e:
        print(f"Error: {str(e)}")
        return None

# Function to send data to ESP32 over serial communication in JSON format
def send_data_to_esp32(data_list, ser):
  try:
    # Check if the serial port is open
    if not ser.is_open:
      print("Serial port is not open.")
      return

    # Send the data to ESP32 as JSON strings
    # for data_dict in data_list:
    #   data_json = json.dumps(data_dict)
    #   ser.write(data_json.encode() + b'\n')
    
    data_json = json.dumps(data_list[0])
    ser.write(data_json.encode() + b'\n')

  except Exception as e:
    print(f"Error sending data to ESP32: {str(e)}")

def close_serial_port(ser):
  if ser and ser.is_open:
    ser.close()
    print(f"Closed Serial Port {ESP_SERIAL_PORT}==>{ESP_BAUD_RATE}")

if __name__ == "__main__":
  # Defined Serial variable for validating on close event
  ser = None
  
  # Configure the serial port with appropriate settings
  try:
    ser = serial.Serial(ESP_SERIAL_PORT, baudrate=ESP_BAUD_RATE)  # Replace 'COMX' with your ESP32's serial port
    close_serial_port(ser)
    ser.open()  # Open the serial port
    
    # Step 1: Read CSV file and get data as a list of dictionaries
    data_list = read_excel_to_dict(excel_file_path)
    print(data_list)

    if data_list:
        # Step 2: Send the data to ESP32 in JSON format
        send_data_to_esp32(data_list, ser)
        pass
    else:
        print("Failed to read CSV file or no data found.")

  except serial.SerialException as e:
      print(f"Serial port error: {str(e)}")

  finally:
    # Close the serial connection
    close_serial_port(ser)