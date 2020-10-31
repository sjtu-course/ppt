import cv2
from matplotlib import pyplot as plt 

# read an image
src = cv2.imread("src_blur.jpg")

_, axs = plt.subplots(1, 3)

# show src image
img = cv2.cvtColor(src, cv2.COLOR_BGR2RGB)
axs[0].imshow(img)

# modest blur
img = cv2.GaussianBlur(src, (5, 5), 0)
img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
axs[1].imshow(img)

# heavy blur 
img = cv2.GaussianBlur(src, (15, 15), 0)
img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
axs[2].imshow(img)

plt.show()
