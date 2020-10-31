import cv2
from matplotlib import pyplot as plt 

src = cv2.imread("src_blur.jpg")

_, axs = plt.subplots(1, 2)

# show src image
img = cv2.cvtColor(src, cv2.COLOR_BGR2RGB)
axs[0].imshow(img)

# blur the image
img_blur = cv2.GaussianBlur(img, (7, 7), 0)

# apply unsharp masking
img = cv2.addWeighted(img, 2, img_blur, -1, 0)
axs[1].imshow(img)

plt.show()
