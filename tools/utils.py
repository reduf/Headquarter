import os

_ROOT = os.path.dirname(os.path.dirname(__file__))

def get_path(*fragments):
    return os.path.join(_ROOT, *fragments)
