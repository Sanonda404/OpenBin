# OpenBin
OpenBin is a smart, contactless dustbin that automatically opens when a user approaches. It monitors its fill level, notifies the user when it reaches 80% capacity, and locks the lid at 95% until the bin is emptied

## Features

- Automatic lid opening
- Ultrasonic distance detection
- Automatic lid closing
- Contactless operation
- Automatic monitoring of fill level
- When >= 85% filled sends notification to clean
- After 95% fill locks lid until cleaned

### **We divide whole in two steps:**
## Step 1: Making smart, contactless dustbin that automatically opens when a user approaches.
   We complete this step using arduino uno later migrate to esp32 in next step.
### Circuit
<img width="1201" height="732" alt="image" src="https://github.com/user-attachments/assets/fdf22e74-38e3-4516-a4bd-ef458787e2f8" />


### How It Works

1. Ultrasonic sensor measures distance.
2. If an object is detected within the threshold...
3. Servo opens the lid.
4. After a delay, the lid closes.

### Demo
https://github.com/user-attachments/assets/3f63b782-c8a4-4109-ab65-01bcb8ebdea7


