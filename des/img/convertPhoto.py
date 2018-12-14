import cPickle as pkl
import os
import numpy as np

import numpy as np
from skimage.io import imsave


PHOTO_FILE = "photo_data"
PIXELS_FILE = "pixel_data.txt"

def unpack_file(fname):
    with open(fname, "r") as f:
        data = pkl.load(f)

    return data


def save_as_image(img_flat, fname):

    img_R = img_flat[0:1024].reshape((32, 32))
    img_G = img_flat[1024:2048].reshape((32, 32))
    img_B = img_flat[2048:3072].reshape((32, 32))
    img = np.dstack((img_R, img_G, img_B))

    imsave(os.path.join(PHOTO_FILE, fname), img)


def main():
    
    data = unpack_file('data_batch_1')

    # for i in range(10000):
    #     img_flat = data["data"][i]
    #     fname = data["filenames"][i]
    #     label = data["labels"][i]

    #     save_as_image(img_flat, fname)
    # print(np.array(data["data"]).shape)
    i = 0 
    with open(PIXELS_FILE, "w") as f:
        for each in data["data"]:
            if i == 10:
                break
            for k in each:
                f.write(str(np.array(k)))
                f.write(' ')   
            i += 1   


if __name__ == "__main__":
    main()
