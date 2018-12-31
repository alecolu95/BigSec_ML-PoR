import random
from bplib import bp
import hashlib

#global parameters
tau = 256# security parameter hash functions

class Data_Owner:

    def __init__(self):
        G = bp.BpGroup()