from numpy import genfromtxt
import matplotlib
from matplotlib import pyplot
from matplotlib.image import imread


pyplot.rcParams["figure.figsize"] = [7.00, 7.00]
pyplot.rcParams["figure.autolayout"] = True

# Read csv file
#my_data = pd.read_csv(r'./output.csv')
input_data = genfromtxt('./output.csv', delimiter=',')
dx_data = genfromtxt('./outputx.csv', delimiter=',')
dy_data = genfromtxt('./outputy.csv', delimiter=',')
theta_data = genfromtxt('./output_theta.csv', delimiter=',')
theta_new_temp_data = genfromtxt('./output_theta_new_temp.csv', delimiter=',')
dnew_data = genfromtxt('./output_dnew.csv', delimiter=',')
dnewtemp_data = genfromtxt('./output_dnew_temp.csv', delimiter=',')
output_data = genfromtxt('./output_final.csv', delimiter=',')

matplotlib.image.imsave('./orig.png', input_data, cmap='gray')
matplotlib.image.imsave('./dx.png', dx_data, cmap='gray')
matplotlib.image.imsave('./dy.png', dy_data, cmap='gray')
matplotlib.image.imsave('./theta.png', theta_data, cmap='gray')
matplotlib.image.imsave('./theta_new_temp.png', theta_new_temp_data, cmap='gray')
matplotlib.image.imsave('./d_new.png', dnew_data, cmap='gray')
matplotlib.image.imsave('./d_new_temp.png', dnewtemp_data, cmap='gray')
matplotlib.image.imsave('./output.png', output_data, cmap='gray')

image_1 = imread('./orig.png')
image_2 = imread('./dx.png')
image_3 = imread('./dy.png')
image_4 = imread('./theta.png')
image_5 = imread('./theta_new_temp.png')
image_6 = imread('./d_new.png')
image_7 = imread('./d_new_temp.png')
image_8 = imread('./output.png')
## plot raw pixel data
#pyplot.subplot(2, 4, 1)
#pyplot.imshow(image_1)
#
#pyplot.subplot(2, 4, 2)
#pyplot.imshow(image_2)
#
#pyplot.subplot(2, 4, 3)
#pyplot.imshow(image_3)
#
#pyplot.subplot(2, 4, 4)
#pyplot.imshow(image_4)
#
#pyplot.subplot(2, 4, 5)
#pyplot.imshow(image_5)
#
#pyplot.subplot(2, 4, 6)
#pyplot.imshow(image_6)
#
#pyplot.subplot(2, 4, 7)
#pyplot.imshow(image_7)
#
#pyplot.subplot(2, 4, 8)
#pyplot.imshow(image_8)
#
## show the figure
#pyplot.show()

fig = pyplot.figure()

ax1 = fig.add_subplot(241)
ax1.title.set_text("orig")
pyplot.imshow(image_1)

ax2 = fig.add_subplot(242)
ax2.title.set_text("dx")
pyplot.imshow(image_2)

ax3 = fig.add_subplot(243)
ax3.title.set_text("dy")
pyplot.imshow(image_3)


ax4 = fig.add_subplot(244)
ax4.title.set_text("theta")
pyplot.imshow(image_4)

ax5 = fig.add_subplot(245)
ax5.title.set_text("theta_new_temp")
pyplot.imshow(image_5)

ax6 = fig.add_subplot(246)
ax6.title.set_text("dnew")
pyplot.imshow(image_6)

ax7 = fig.add_subplot(247)
ax7.title.set_text("dnew_temp")
pyplot.imshow(image_7)

ax8 = fig.add_subplot(248)
ax8.title.set_text("Output")
pyplot.imshow(image_8)

pyplot.show()