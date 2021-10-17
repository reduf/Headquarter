import os

_ROOT = os.path.dirname(os.path.dirname(__file__))

def get_path(path):
    return os.path.join(_ROOT, path)
