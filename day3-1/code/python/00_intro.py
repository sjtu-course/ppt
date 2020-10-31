import cv2
from matplotlib import pyplot as plt 

# read an image
src = cv2.imread("src_blur.jpg")
# convert an image
img = cv2.cvtColor(src, cv2.COLOR_BGR2RGB)
# show an image
plt.imshow(img)
plt.show()
