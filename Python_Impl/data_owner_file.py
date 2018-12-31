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

        #randomize hash for key server
        pow(self.G.gen1(),alpha)

        #key server signing

        #remove key server randomization
        (ks_first_pk, ks_second_pk) = self.keyserver.get_public_key_pair()
        pow(ks_first_pk,-alpha)

        #verify with bilinear pairing

        #add new randomization for cloud
        pow(self.G.gen1(),beta)

        #cloud signing

        #remove cloud randomization
        (c_first_pk, c_second_pk) = self.cloud.get_public_key_pair()
        pow(c_first_pk,-beta)

        #verify with bilinear pairing


