import random
from bplib import bp
import hashlib
from cloud_file import Cloud
from keyserver_file import Keyserver

#global parameters
tau = 256# security parameter hash functions
random_size = 50 # security parameters, size of

class Data_Owner:

    def __init__(self):
        """
        generate bilinear pairing, cloud, keyserver
        """
        self.G = bp.BpGroup()
        self.cloud = Cloud(self.G.gen1(),self.G.gen2())
        self.keyserver = Keyserver(self.G.gen1(),self.G.gen2())


    def gen_key(self,file):
        alpha = random.getrandbits(random_size)
        beta = random.getrandbits(random_size)

        #hash file