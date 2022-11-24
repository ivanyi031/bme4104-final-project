import cv2
import numpy as np

# capture an image
cap = cv2.VideoCapture("http://192.168.136.125:8081/?action=stream")
fetch_success = False
while not fetch_success:
    fetch_success, img = cap.read()

# load the image
img = np.array(img)  # BGR format

# ? extract circular region
# ! use dummy region; central 24x24
cx, cy, _ = np.array(img.shape) // 2
w, h = 24, 24
roi = img[cy - h // 2 : cy + h // 2, cx - w // 2 : cx + w // 2, :]

# convert BGR to HSV
hsv = cv2.cvtColor(roi, cv2.COLOR_BGR2HSV_FULL)

# determine the color
hsv_mean = hsv.reshape(-1, 3).mean(axis=0)
print(hsv_mean)

# print the color
if 0 <= hsv_mean[0] <= 50:
    print("red")
elif 140 <= hsv_mean[0] <= 160:
    print("blue")
else:
    print("idk")
