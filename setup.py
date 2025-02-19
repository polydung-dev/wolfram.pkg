from setuptools import Extension, setup
setup(
    name = "wolfram",
    version = "0.4.1",
    description = "Wolfram Elementary Cellular Automata",
    author = "PolyDung Dev",
    author_email = "polydung.dev@proton.me",
    url = "https://github.com/polydung-dev/wolfram",
    ext_modules = [
        Extension(
            name = "wolfram",
            sources = ["src/wolframmodule.c", "src/eca.c", "src/str.c"],
            depends = ["src/eca.h", "src/str.h"]
        )
    ]
)
