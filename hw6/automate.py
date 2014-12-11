import os
import subprocess

rootdir = './images'

for cat in os.listdir(rootdir):
	if (cat[0] != '.'):
		subdir = rootdir + '/' + cat
		for img in os.listdir(subdir):
			if (len(img) == len(cat) + 6):
				img_dir = subdir + '/' + img
				clustered = img_dir[:-4] + "_clustered.pgm"
				connected = img_dir[:-4] + "_connected.ppm"
				subprocess.call(["./kmeans", img_dir])
				subprocess.call(["./conrgn", "c", clustered])
				subprocess.call(["./regions", img_dir, connected])