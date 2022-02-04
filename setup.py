import re
import sys
from setuptools import find_packages

try:
    from skbuild import setup
except ImportError:
    print(
        "Please update pip, you need pip 10 or greater,\n"
        " or you need to install the PEP 518 requirements in pyproject.toml yourself",
        file=sys.stderr,
    )
    raise

# Add pybind11 if it is installed
cmake_args = []
try:
    from pybind11 import get_cmake_dir
except ImportError:
    pass
else:
    cmake_args.append("-Dpybind11_DIR:PATH=" + get_cmake_dir())

# Read version from package.xml
with open("package.xml", "r") as f:
    __version__ = re.search("<version>([0-9]+.[0-9]+.[0-9]+)</version>", f.read()).group(1)

setup(
    name="abb_librws",
    version=__version__,
    description="ABB Robot Web Services (RWS) Python Bindings",
    author="Mike Danielczuk",
    author_email="michael.danielczuk@gmail.com",
    packages=find_packages(where="python"),
    package_dir={"": "python"},
    cmake_install_dir="python/abb_librws",
    cmake_args=cmake_args,
)
