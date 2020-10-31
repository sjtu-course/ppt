import cv2
from matplotlib import pyplot as plt 

src = cv2.imread("src_edge.jpg")

_, axs = plt.subplots(1, 4)

# show src image
img = cv2.cvtColor(src, cv2.COLOR_BGR2RGB)
axs[0].imshow(img)

# preprocess by blurring and grayscale
img = cv2.GaussianBlur(src, (7, 7), 0)
img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
axs[1].imshow(cv2.cvtColor(img, cv2.COLOR_GRAY2RGB))

# find binary image with thresholding
_, img_thresh = cv2.threshold(img, 80, 255, cv2.THRESH_BINARY)
axs[2].imshow(cv2.cvtColor(img_thresh, cv2.COLOR_GRAY2RGB))

# find binary image with edges
img_canny = cv2.Canny(img, threshold1=90, threshold2=110)
axs[3].imshow(cv2.cvtColor(img_canny, cv2.COLOR_GRAY2RGB))

plt.show()
