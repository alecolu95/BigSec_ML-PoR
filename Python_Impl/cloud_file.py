import random

#global parameters
sk_size = 50

class Cloud:

    def __init__(self, generator_g1, generator_g2):
        """
        Generates private key and corresponding public key pair
        :param generator_g1: generator of Group g1
        :param generator_g2: generator of Group g2
        """
        #generate private key
        self.private_key = random.getrandbits(sk_size)
        self.public_key_pair = (pow(generator_g1,self.private_key),pow(generator_g2,self.private_key))

    def get_public_key_pair(self):
        """
        Returns the public key pair of the Cloud instance
        :return: public key pair
        """
        return self.public_key_pair